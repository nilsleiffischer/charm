/*
 This is the public interface file to the armci library
 The interface is derived from the header file of the same name by the
 original developer(?) Jarek Nieplocha of ARMCI at Pacific Northwest 
 National Laboratory.
 
 Original version by Chee Wai Lee, 2002
 Updated version by Chao Huang, 2005
*/

/* 
 * missing interface for:
  13. Collective Operations
  14. System Configuration
 */

#ifndef _ARMCI_H
#define _ARMCI_H

#include "tcharmc.h" /* for TCHARM_Element */

#ifdef __cplusplus
extern "C" {
#endif

/* Datatypes */
#define ARMCI_ACC_INT	1
#define ARMCI_ACC_LNG	2
#define ARMCI_ACC_FLT	3
#define ARMCI_ACC_DBL	4
#define ARMCI_ACC_CPL	5
#define ARMCI_ACC_DCPL	6

#define ARMCI_MAX_STRIDE_LEVEL 8

/* operations */
#define ARMCI_FETCH_AND_ADD 		1
#define ARMCI_FETCH_AND_ADD_LONG	2
#define ARMCI_SWAP			3
#define ARMCI_SWAP_LONG			4

/* user's thread start routine */
int armciStart(int argc, char **argv);
  
/* redefine global variables used by armci */
#define armci_me TCHARM_Element()
#define armci_master 0
extern int armci_nproc;

/* structures */
typedef struct {
  void **src_ptr_array;
  void **dst_ptr_array;
  int  ptr_array_len;
  int bytes;
} armci_giov_t;

typedef struct {
  int data[4];
  double dummy;
} armci_hdl_t;

/* virtual processor Aggregate Remote Memory Copy Interface (ARMCI) */

/* ***************** */
/* system operations */
/* 0 */
int ARMCI_Init(void);
/* 0 */
int ARMCI_Finalize(void);
void ARMCI_Error(char *msg, int code);
void ARMCI_Cleanup(void);

/* ****************************** */
/* vector IO-type copy operations */
int ARMCI_GetV(
	        armci_giov_t darr[], /* descriptor array */
	        int len,              /* length of descriptor array */
	        int proc              /* remote process(or) ID */
	      );

int ARMCI_NbGetV(
		armci_giov_t *dsrc_arr,
		int arr_len,
		int proc,
		armci_hdl_t* handle
		);

int ARMCI_PutV(
	        armci_giov_t darr[], /* descriptor array */
	        int len,              /* length of descriptor array */
	        int proc              /* remote process(or) ID */
	      );

int ARMCI_NbPutV(
		armci_giov_t *dsrc_arr,
		int arr_len,
		int proc,
		armci_hdl_t* handle
	        );

int ARMCI_AccV(
	        int op,                /* operation code */
	        void *scale,          /* scaling factor for accumulate */
	        armci_giov_t darr[], /* descriptor array */
	        int len,              /* length of descriptor array */
	        int proc              /* remote process(or) ID */
	      );

int ARMCI_NbAccV(
		int datatype, 
		void *scale, 
		armci_giov_t *dsrc_arr, 
		int arr_len, 
		int proc, 
		armci_hdl_t* handle
		);

/* ********************* */
/* basic copy operations */
/* 0 */ 
int ARMCI_Put(void *src, void* dst, int bytes, int proc);
/* 0 */ 
int ARMCI_NbPut(void *src, void* dst, int bytes, int proc, 
		armci_hdl_t *handle);
/* 0 */ 
int ARMCI_Get(void *src, void* dst, int bytes, int proc);
/* 2 */ 
int ARMCI_NbGet(void *src, void* dst, int bytes, int proc, 
		armci_hdl_t *handle);
  
int ARMCI_Acc(int datatype, void *scale, void* src, void* dst, int bytes, int proc);

int ARMCI_NbAcc(int datatype, void *scale, void* src, void* dst, int bytes, int proc, 
                armci_hdl_t* handle);

/* *********************** */
/* strided copy operations */
/* 0 */ 
int ARMCI_PutS(
	        void *src_ptr,         /* ptr to 1st segment at source */
	        int src_stride_arr[], /* array of strides at source  */
	        void* dst_ptr,         /* ptr to 1st segment at dest */
	        int dst_stride_arr[], /* array of strides at destination */
	        int count[],           /* number of units at each stride  */
                                          /* level count[0]=bytes  */
	        int stride_levels,    /* number of stride levels */
	        int proc                /* remote process(or) ID  */
	        );

/* 1 */ 
int ARMCI_NbPutS(
		 void *src_ptr,         /* ptr to 1st segment at source */
		 int src_stride_arr[], /* array of strides at source  */
		 void* dst_ptr,         /* ptr to 1st segment at dest */
		 int dst_stride_arr[], /* array of strides at destination */
		 int count[],           /* number of units at each stride  */
		                          /* level count[0]=bytes  */
		 int stride_levels,    /* number of stride levels */
		 int proc,              /* remote process(or) ID  */
		 armci_hdl_t *handle   /* pointer to descriptor associated */
		                          /* with a particular non-blocking */
                                            /* transfer. Passing NULL value */
		                          /* makes this function do an */
                                            /* implicit handle non-blocking */
                                            /* transfer */
		 );

/* 0 */ 
int ARMCI_GetS(
	        void *src_ptr,         /* pointer to 1st segment at source */
	        int src_stride_arr[], /* array of strides at source */
	        void* dst_ptr,         /* ptr to 1st segment at destination */
	        int dst_stride_arr[], /* array of strides at destination */
	        int count[],           /* number of units at each stride  */
	                                 /* level count[0]=bytes */
	        int stride_levels,    /* number of stride levels */
	        int proc                /* remote process(or) ID */
	        );

int ARMCI_NbGetS(
		void *src_ptr, 
		int src_stride_ar[], 
		void* dst_ptr, 
		int dst_stride_ar[], 
		int count[], 
		int stride_levels, 
		int proc, 
		armci_hdl_t* handle
		);

int ARMCI_AccS(
	        int  optype,           /* operation */
	        void *scale,           /* scale factor x += scale*y */
	        void *src_ptr,         /* pointer to 1st segment at source */
	        int src_stride_arr[], /* array of strides at source */
	        void* dst_ptr,         /* ptr to 1st segment at destination */
	        int dst_stride_arr[], /* array of strides at destination */
	        int count[],           /* number of units at each stride  */
	                                 /* level count[0]=bytes */
	        int stride_levels,    /* number of stride levels */
	        int proc                /* remote process(or) ID */
	        );

int ARMCI_NbAccS(
		int datatype, 
		void *scale, 
		void* src_ptr,
		int src_stride_ar[], 
		void* dst_ptr, 
		int dst_stride_ar[], 
		int count[],  
		int stride_levels, 
		int proc, 
		armci_hdl_t* handle
		);

/* ******************************************** */
/* Register Originated Data Transfer Operations */
int ARMCI_PutValueLong(long src, void* dst, int proc);
int ARMCI_PutValueInt(int src, void* dst, int proc);
int ARMCI_PutValueFloat(float src, void* dst, int proc);
int ARMCI_PutValueDouble(double src, void* dst, int proc);
int ARMCI_NbPutValueLong(long src, void* dst, int proc, armci_hdl_t* handle);
int ARMCI_NbPutValueInt(int src, void* dst, int proc, armci_hdl_t* handle);
int ARMCI_NbPutValueFloat(float src, void* dst, int proc, armci_hdl_t* handle);
int ARMCI_NbPutValueDouble(double src, void* dst, int proc, armci_hdl_t* handle);
long ARMCI_GetValueLong(void *src, int proc);
int ARMCI_GetValueInt(void *src, int proc);
float ARMCI_GetValueFloat(void *src, int proc);
double ARMCI_GetValueDouble(void *src, int proc);

/* ********************************** */
/* Functions for Non-blocking support */
/* 0 */
int ARMCI_Wait(armci_hdl_t *handle);
int ARMCI_WaitProc(int proc);
int ARMCI_WaitAll();
/* 0 */
int ARMCI_Test(armci_hdl_t *handle);
/* 0 */
int ARMCI_Barrier();

/* ******************************************************** */
/* fence operations (for synchronizing with put operations) */
/* 0 */
int ARMCI_Fence(int proc);
/* 0 */
int ARMCI_AllFence(void);

/* ***************** */
/* memory operations */
/* 0 */
int ARMCI_Malloc(void* ptr_arr[], int bytes);
/* 0 */
int ARMCI_Free(void *ptr);
/* 0 */
void *ARMCI_Malloc_local(int bytes);
/* 0 */
int ARMCI_Free_local(void *ptr);
 
/* *********************************** */
/* Aggregated Data Transfer Operations */
void ARMCI_SET_AGGREGATE_HANDLE (armci_hdl_t* handle);
void ARMCI_UNSET_AGGREGATE_HANDLE (armci_hdl_t* handle);

/* ************************************* */
/* Atomic and Synchronization Operations */
int ARMCI_Rmw(int op, int *ploc, int *prem, int extra, int proc);
int ARMCI_Create_mutexes(int num);
int ARMCI_Destroy_mutexes(void);
void ARMCI_Lock(int mutex, int proc);
void ARMCI_Unlock(int mutex, int proc);

/* ***************** */
/* notify operations */

/* 1 */
int armci_notify(int proc);
/* 1 */
int armci_notify_wait(int proc, int *pval);

/* this is highly platform specific and I don't think it needs to be
   included. */
/* extern void ARMCI_Set_shm_limit(unsigned long shmemlimit); */
/* extern int ARMCI_Uses_shm(); */

#ifdef __cplusplus
};
#endif

#define ARMCI_INIT_HANDLE(hdl) do {((double *)((hdl)->data))[0]=0; \
  ((double *)((hdl)->data))[1]=0; }while(0)

#endif
