#ifndef _FEM_H
#define _FEM_H

#include <stdio.h>
#include "fem.decl.h"

extern CkChareID _mainhandle;
extern CkArrayID _femaid;
extern unsigned int _nchunks;

#define CHK(p) do{if((p)==0)CkAbort("FEM>Memory Allocation failure.");}while(0)

// base types: keep in sync with femf.h
#define FEM_BYTE   0
#define FEM_INT    1
#define FEM_REAL   2
#define FEM_DOUBLE 3

// reduction operations: keep in synch with femf.h
#define FEM_SUM 0
#define FEM_MAX 1
#define FEM_MIN 2

// element types
#define FEM_TRIANGULAR    1
#define FEM_TETRAHEDRAL   2
#define FEM_HEXAHEDRAL    3
#define FEM_QUADRILATERAL 4

// temporary Datatype representation
// will go away once MPI user-defined datatypes are ready
struct DType {
  int base_type;
  int vec_len;
  int init_offset; // offset of field in bytes from the beginning of data
  int distance; // distance in bytes between successive field values
  DType(void) {}
  DType(const DType& dt) : 
    base_type(dt.base_type), vec_len(dt.vec_len), init_offset(dt.init_offset),
    distance(dt.distance) {}
  void operator=(const DType& dt) {
    base_type = dt.base_type; 
    vec_len = dt.vec_len; 
    init_offset = dt.init_offset;
    distance = dt.distance;
  }
  DType( const int b,  const int v=1,  const int i=0,  const int d=0) : 
    base_type(b), vec_len(v), init_offset(i) {
    distance = (d ? d : length());
  }
  int length(const int nitems=1) const {
    int blen;
    switch(base_type) {
      case FEM_BYTE : blen = 1; break;
      case FEM_INT : blen = sizeof(int); break;
      case FEM_REAL : blen = sizeof(float); break;
      case FEM_DOUBLE : blen = sizeof(double); break;
    }
    return blen * vec_len * nitems;
  }
};

class DataMsg : public CMessage_DataMsg
{
 public:
  int from;
  int dtype;
  void *data;
  int tag;
  DataMsg(int t, int f, int d) : 
    tag(t), from(f), dtype(d) { data = (void*) (this+1); }
  DataMsg(void) { data = (void*) (this+1); }
  static void *pack(DataMsg *);
  static DataMsg *unpack(void *);
  static void *alloc(int, size_t, int*, int);
};

class main : public Chare
{
  int numdone;
  int isMeshSet;
 public:
  main(CkArgMsg *);
  void done(void);
  void setMesh(int,int,int,int*,int*);
};

class ChunkMsg : public CMessage_ChunkMsg {
 public:
  int nnodes, nelems, npes, nconn;
  int *gNodeNums; // gNodeNums[nnodes]
  int *primaryPart; // primaryPart[nnodes]
  int *gElemNums; // gElemNums[nelems]
  int *conn; // conn[nelems][nconn]
  int *peNums; // peNums[npes]
  int *numNodesPerPe; // numNodesPerPe[npes]
  int *nodesPerPe; // nodesPerPe[npes][nodesPerPe[i]]
  static void *pack(ChunkMsg *);
  static ChunkMsg *unpack(void *);
};

#define MAXDT 20

class chunk : public ArrayElement1D
{
  int numNodes; // number of local nodes
  int numElems; // number of local elements
  int numPes; // number of Pes I need to communicate with
  int *gNodeNums; // global node numbers for local nodes [numNodes]
  int *isPrimary; // 1 true if primary (i.e. owner of a shared node)
  int numNodesPerElem; // number of Nodes per element *a constant*
  int *gElemNums; // global element numbers for local elements [numElems]
  int *conn; // elem->node connectivity, a 2-D array stored in row-order
  int *peNums; // which Pes I need to communicate with [numPes]
  int *numNodesPerPe; // How many Pes I need to communicate [numPes]
  int **nodesPerPe; // Pes->nodes map
  int *gPeToIdx; // which local index does the global PeNum map to [TotalPes]

  DType dtypes[MAXDT];
  int ntypes;

  CmmTable messages; // messages to be processed
  int wait_for; // which tag is tid waiting for ? 0 if not waiting
  CthThread tid; // waiting thread, 0 if no one is waiting

  int seqnum; // sequence number for update operation
  int nRecd; // number of messages received for this seqnum
  void *curbuf; // data addr for current update operation

 public:

  int doneCalled;
  chunk(void);
  chunk(CkMigrateMessage *) {}
  void run(void);
  void run(ChunkMsg*);
  void recv(DataMsg *);
  void result(DataMsg *);
  int new_DT(int base_type, int vec_len=1, int init_offset=0, int distance=0) {
    if(ntypes==MAXDT) {
      CkAbort("FEM: registered datatypes limit exceeded.");
    }
    dtypes[ntypes] = DType(base_type, vec_len, init_offset, distance);
    ntypes++;
    return ntypes-1;
  }
  void update(int fid, void *nodes);
  void reduce_field(int fid, void *nodes, void *outbuf, int op);
  void reduce(int fid, void *inbuf, void *outbuf, int op);
  void readField(int fid, void *nodes, char *fname);
  int id(void) { return thisIndex; }
  int total(void) { return numElements; }
  void print(void);
 private:
  FILE *fp;
  void update_field(DataMsg *);
  void send(int fid, void *nodes);
  void readNodes(ChunkMsg *msg=0);
  void readElems(ChunkMsg *msg=0);
  void readComm(ChunkMsg *msg=0);
  void readChunk(ChunkMsg *msg=0);
};

void FEM_Done(void);
int FEM_Create_Field(int base_type, int vec_len, int init_offset, int distance);
void FEM_Update_Field(int fid, void *nodes);
void FEM_Reduce_Field(int fid, void *nodes, void *outbuf, int op);
void FEM_Reduce(int fid, void *inbuf, void *outbuf, int op);
int FEM_My_Partition(void);
int FEM_Num_Partitions(void);
void FEM_Read_Field(int fid, void *nodes, char *fname);
void FEM_Print(char *str);
void FEM_Set_Mesh(int nelem, int nnodes, int ctype, int* connmat);
void FEM_Set_Mesh_Transform(int nelem, int nnodes, int ctype, int* connmat, 
                            int *permute);
void FEM_Print_Partition(void);

// Fortran Bindings
#if CMK_FORTRAN_USES_ALLCAPS
extern "C" int FEM_CREATE_FIELD(int *bt, int *vl, int *io, int *d);
extern "C" void FEM_UPDATE_FIELD(int *fid, void *nodes);
extern "C" void FEM_REDUCE_FIELD(int *fid, void *nodes, void *outbuf, int *op);
extern "C" void FEM_REDUCE(int *fid, void *inbuf, void *outbuf, int *op);
extern "C" int FEM_MY_PARTITION(void);
extern "C" int FEM_NUM_PARTITIONS(void);
// FIXME: correct fortran-c interoperability issue for passing character arrays
extern "C" void FEM_READ_FIELD(int *fid, void *nodes, char *fname, int len);
extern "C" void FEM_PRINT(char *str, int len);
extern "C" void FEM_SET_MESH(int *nelem,int *nnodes,int *ctype,int *connmat);
extern "C" void FEM_SET_MESH_TRANSFORM(int *nelem,int *nnodes,int *ctype,
                                       int *connmat, int *permute);
extern "C" void FEM_PRINT_PARTITION(void);
extern "C" int OFFSETOF(void *, void *);
// to be provided by the application
extern "C" void INIT(void);
extern "C" void DRIVER(int *, int *, int *, int *, int *, int *);
extern "C" void FINALIZE(void);
#else
extern "C" int fem_create_field_(int *bt, int *vl, int *io, int *d);
extern "C" void fem_update_field_(int *fid, void *nodes);
extern "C" void fem_reduce_field_(int *fid, void *nodes, void *outbuf, int *op);
extern "C" void fem_reduce_(int *fid, void *inbuf, void *outbuf, int *op);
extern "C" int fem_my_partition_(void);
extern "C" int fem_num_partitions_(void);
// FIXME: correct fortran-c interoperability issue for passing character arrays
extern "C" void fem_read_field_(int *fid, void *nodes, char *fname, int len);
extern "C" void fem_print_(char *str, int len);
extern "C" void fem_set_mesh_(int *nelem,int *nnodes,int *ctype,int *connmat);
extern "C" void fem_set_mesh_transform_(int *nelem,int *nnodes,int *ctype,
                                        int *connmat, int *permute);
extern "C" void fem_print_partition_(void);
extern "C" int offsetof_(void *, void *);
// to be provided by the application
extern "C" void init_(void);
extern "C" void driver_(int *, int *, int *, int *, int *, int *);
extern "C" void finalize_(void);
#endif


#endif
