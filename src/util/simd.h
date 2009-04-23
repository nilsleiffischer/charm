#ifndef __SIMD_H__
#define __SIMD_H__


#if defined(__SSE2__)
  #include "emmintrin.h"
#endif

#if CMK_CELL_SPE != 0
  #include "spu_intrinsics.h"
#else
  #include "math.h"
#endif

#if defined(__VEC__)
  #include "altivec.h"
#endif


/* Solaris does not support sqrtf (float), so just map it to sqrt (double) instead */
#if !CMK_HAS_SQRTF
  #define sqrtf(a) ((float)(sqrt((double)(a))))
#endif


/* Flags to force architecture specific SIMD instructions off */
#define FORCE_NO_SSE       (0)
#define FORCE_NO_ALTIVEC   (0)
#define FORCE_NO_SPE_SIMD  (0)


/***** Math Constants *****/
#define __SIMD__CONSTANT_PI      (3.141592653589793)
#define __SIMD__CONSTANT_E       (2.718281828459045)
#define __SIMD__CONSTANT_SQRT_2  (1.414213562373095)
/* TODO | FIXME - Added intrinsics below for loading/creating vectors with these values */


/* TODO | FIXME - Find platform independent way of ensuring alignment
 * (using __attribute__((aligned(XXX))) doesn't seem to work in net-win and
 * net-sol builds).  Just to be safe since compilers should do this anyway.
 */

/* TODO | FIXME - Add a function that will test the functionality of the
 * various operations defined by these abstractions and somehow tie this test
 * into the nightly build to ensure these operations give correct results.
 */


/*******************************************************************************
 *******************************************************************************
 ***** Generic C Implementation
 *******************************************************************************
 *******************************************************************************/

/*@{*/

/* NOTE: This is declared first so any architecture specific implementations
 *   can simply use the generic functions for specific data types or operations
 *   that they do not implement.
 */
 
/***** Data Types *****/
/*
 * typedef struct __vec_16_c  {           char v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15; } __vec16c;
 * typedef struct __vec_16_uc {  unsigned char v0, v1, v2, v3, v4, v5, v6, v7, v8, v9, v10, v11, v12, v13, v14, v15; } __vec16uc;
 * typedef struct __vec_8_s  {          short v0, v1, v2, v3, v4, v5, v6, v7; } __vec8s;
 * typedef struct __vec_8_us { unsigned short v0, v1, v2, v3, v4, v5, v6, v7; } __vec8us;
 * typedef struct __vec_4_ui {   unsigned int v0, v1, v2, v3; } __vec4ui;
 */


typedef struct __vec_4_i  {    int v0, v1, v2, v3; }  __vec4i;
typedef struct __vec_4_f  {  float v0, v1, v2, v3; }  __vec4f;
typedef struct __vec_2_lf { double v0, v1;         } __vec2lf;


/***** Insert *****/
inline  __vec4i  __vinsert4i( __vec4i v, const    int s, const int i) {  __vec4i r = v;    int* rPtr = (   int*)(&r); rPtr[i] = s; return r; }
inline  __vec4f  __vinsert4f( __vec4f v, const  float s, const int i) {  __vec4f r = v;  float* rPtr = ( float*)(&r); rPtr[i] = s; return r; }
inline __vec2lf __vinsert2lf(__vec2lf v, const double s, const int i) { __vec2lf r = v; double* rPtr = (double*)(&r); rPtr[i] = s; return r; }

/***** Extract *****/
inline    int  __vextract4i( __vec4i v, const int i) {    int* vPtr = (   int*)(&v); return vPtr[i]; }
inline  float  __vextract4f( __vec4f v, const int i) {  float* vPtr = ( float*)(&v); return vPtr[i]; }
inline double __vextract2lf(__vec2lf v, const int i) { double* vPtr = (double*)(&v); return vPtr[i]; }

/***** Set *****/
inline  __vec4i  __vset4i(const    int a) {  __vec4i r; r.v0 = r.v1 = r.v2 = r.v3 = a; return r; }
inline  __vec4f  __vset4f(const  float a) {  __vec4f r; r.v0 = r.v1 = r.v2 = r.v3 = a; return r; }
inline __vec2lf __vset2lf(const double a) { __vec2lf r; r.v0 = r.v1 =               a; return r; }

/* NOTE: Would it be better to generate the constants instead of read them from memory in the generic version? */

/***** Constant Zero *****/
const  __vec4i  __const_vzero4i = {   0 ,   0 ,   0 ,   0  };
const  __vec4f  __const_vzero4f = { 0.0f, 0.0f, 0.0f, 0.0f };
const __vec2lf __const_vzero2lf = { 0.0 , 0.0              };

/***** Constant One *****/
const  __vec4i  __const_vone4i = {   1 ,   1 ,   1 ,   1  };
const  __vec4f  __const_vone4f = { 1.0f, 1.0f, 1.0f, 1.0f };
const __vec2lf __const_vone2lf = { 1.0 , 1.0              };

/***** Constant Two *****/
const  __vec4i  __const_vtwo4i = {   2 ,   2 ,   2 ,   2  };
const  __vec4f  __const_vtwo4f = { 2.0f, 2.0f, 2.0f, 2.0f };
const __vec2lf __const_vtwo2lf = { 2.0 , 2.0              };

/***** Constant Negative One *****/
const  __vec4i  __const_vnegone4i = {   -1 ,   -1 ,   -1 ,   -1  };
const  __vec4f  __const_vnegone4f = { -1.0f, -1.0f, -1.0f, -1.0f };
const __vec2lf __const_vnegone2lf = { -1.0 , -1.0                };

/* TODO | FIXME - Try to create constants such that it does not require a
 * memory operations to access the constants (like the SSE constants).
 */

/***** Rotate *****/
inline  __vec4i  __vroth4i(const  __vec4i a, int s) {  __vec4i b;    int* a_ptr = (   int*)(&a);    int* b_ptr = (   int*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0-s)&0x3]; b_ptr[1] = a_ptr[(1-s)&0x3]; b_ptr[2] = a_ptr[(2-s)&0x3]; b_ptr[3] = a_ptr[(3-s)&0x3]; return b; }
inline  __vec4f  __vroth4f(const  __vec4f a, int s) {  __vec4f b;  float* a_ptr = ( float*)(&a);  float* b_ptr = ( float*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0-s)&0x3]; b_ptr[1] = a_ptr[(1-s)&0x3]; b_ptr[2] = a_ptr[(2-s)&0x3]; b_ptr[3] = a_ptr[(3-s)&0x3]; return b; }
inline __vec2lf __vroth2lf(const __vec2lf a, int s) { __vec2lf b; double* a_ptr = (double*)(&a); double* b_ptr = (double*)(&b); s &= 0x1; b_ptr[0] = a_ptr[(0-s)&0x1]; b_ptr[1] = a_ptr[(1-s)&0x1]; return b; }
inline  __vec4i  __vrotl4i(const  __vec4i a, int s) {  __vec4i b;    int* a_ptr = (   int*)(&a);    int* b_ptr = (   int*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0+s)&0x3]; b_ptr[1] = a_ptr[(1+s)&0x3]; b_ptr[2] = a_ptr[(2+s)&0x3]; b_ptr[3] = a_ptr[(3+s)&0x3]; return b; }
inline  __vec4f  __vrotl4f(const  __vec4f a, int s) {  __vec4f b;  float* a_ptr = ( float*)(&a);  float* b_ptr = ( float*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0+s)&0x3]; b_ptr[1] = a_ptr[(1+s)&0x3]; b_ptr[2] = a_ptr[(2+s)&0x3]; b_ptr[3] = a_ptr[(3+s)&0x3]; return b; }
inline __vec2lf __vrotl2lf(const __vec2lf a, int s) { __vec2lf b; double* a_ptr = (double*)(&a); double* b_ptr = (double*)(&b); s &= 0x1; b_ptr[0] = a_ptr[(0+s)&0x1]; b_ptr[1] = a_ptr[(1+s)&0x1]; return b; }

/***** Addition *****/
inline  __vec4i  __vadd4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = a.v0 + b.v0; r.v1 = a.v1 + b.v1; r.v2 = a.v2 + b.v2; r.v3 = a.v3 + b.v3; return r; }
inline  __vec4f  __vadd4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; r.v0 = a.v0 + b.v0; r.v1 = a.v1 + b.v1; r.v2 = a.v2 + b.v2; r.v3 = a.v3 + b.v3; return r; }
inline __vec2lf __vadd2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; r.v0 = a.v0 + b.v0; r.v1 = a.v1 + b.v1;                                         return r; }

/***** Subtraction *****/
inline  __vec4i  __vsub4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = a.v0 - b.v0; r.v1 = a.v1 - b.v1; r.v2 = a.v2 - b.v2; r.v3 = a.v3 - b.v3; return r; }
inline  __vec4f  __vsub4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; r.v0 = a.v0 - b.v0; r.v1 = a.v1 - b.v1; r.v2 = a.v2 - b.v2; r.v3 = a.v3 - b.v3; return r; }
inline __vec2lf __vsub2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; r.v0 = a.v0 - b.v0; r.v1 = a.v1 - b.v1;                                         return r; }

/***** Multiplication *****/
inline  __vec4i  __vmul4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = a.v0 * b.v0; r.v1 = a.v1 * b.v1; r.v2 = a.v2 * b.v2; r.v3 = a.v3 * b.v3; return r; }
inline  __vec4f  __vmul4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; r.v0 = a.v0 * b.v0; r.v1 = a.v1 * b.v1; r.v2 = a.v2 * b.v2; r.v3 = a.v3 * b.v3; return r; }
inline __vec2lf __vmul2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; r.v0 = a.v0 * b.v0; r.v1 = a.v1 * b.v1;                                         return r; }

/***** Division *****/
inline  __vec4i  __vdiv4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = a.v0 / b.v0; r.v1 = a.v1 / b.v1; r.v2 = a.v2 / b.v2; r.v3 = a.v3 / b.v3; return r; }
inline  __vec4f  __vdiv4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; r.v0 = a.v0 / b.v0; r.v1 = a.v1 / b.v1; r.v2 = a.v2 / b.v2; r.v3 = a.v3 / b.v3; return r; }
inline __vec2lf __vdiv2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; r.v0 = a.v0 / b.v0; r.v1 = a.v1 / b.v1;                                         return r; }

/***** Fused Multiply Add *****/
inline  __vec4i  __vmadd4i(const  __vec4i a, const  __vec4i b, const  __vec4i c) {  __vec4i r; r.v0 = a.v0 * b.v0 + c.v0; r.v1 = a.v1 * b.v1 + c.v1; r.v2 = a.v2 * b.v2 + c.v2; r.v3 = a.v3 * b.v3 + c.v3; return r; }
inline  __vec4f  __vmadd4f(const  __vec4f a, const  __vec4f b, const  __vec4f c) {  __vec4f r; r.v0 = a.v0 * b.v0 + c.v0; r.v1 = a.v1 * b.v1 + c.v1; r.v2 = a.v2 * b.v2 + c.v2; r.v3 = a.v3 * b.v3 + c.v3; return r; }
inline __vec2lf __vmadd2lf(const __vec2lf a, const __vec2lf b, const __vec2lf c) { __vec2lf r; r.v0 = a.v0 * b.v0 + c.v0; r.v1 = a.v1 * b.v1 + c.v1;                                                       return r; }

/***** Reciprocal *****/
/* TODO | FIXME  - See if there is a better way to do this (few cycles and avoid the memory load) */
inline  __vec4f  __vrecip4f(const  __vec4f a) {  __vec4f r; r.v0 = 1.0f / a.v0; r.v1 = 1.0f / a.v1; r.v2 = 1.0f / a.v2; r.v3 = 1.0f / a.v3; return r; }
inline __vec2lf __vrecip2lf(const __vec2lf a) { __vec2lf r; r.v0 = 1.0 / a.v0; r.v1 = 1.0 / a.v1; return r; }

/***** Square Root *****/
inline  __vec4f  __vsqrt4f(const  __vec4f a) {  __vec4f r; r.v0 = sqrtf(a.v0); r.v1 = sqrtf(a.v1); r.v2 = sqrtf(a.v2); r.v3 = sqrtf(a.v3); return r; }
inline __vec2lf __vsqrt2lf(const __vec2lf a) { __vec2lf r; r.v0 = sqrt(a.v0); r.v1 = sqrt(a.v1); return r; }

/***** Reciprocal Square Root *****/
inline  __vec4f  __vrsqrt4f(const  __vec4f a) {  __vec4f r; r.v0 = 1.0f / sqrtf(a.v0); r.v1 = 1.0f / sqrtf(a.v1); r.v2 = 1.0f / sqrtf(a.v2); r.v3 = 1.0f / sqrtf(a.v3); return r; }
inline __vec2lf __vrsqrt2lf(const __vec2lf a) { __vec2lf r; r.v0 = 1.0 / sqrt(a.v0); r.v1 = 1.0 / sqrt(a.v1); return r; }

/***** Not *****/
inline  __vec4i  __vnot4i(const  __vec4i a) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); rPtr[0] = aPtr[0] ^ 0x0; rPtr[1] = aPtr[1] ^ 0x0; rPtr[2] = aPtr[2] ^ 0x0; rPtr[3] = aPtr[3] ^ 0x0; return r; }
inline  __vec4f  __vnot4f(const  __vec4f a) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); rPtr[0] = aPtr[0] ^ 0x0; rPtr[1] = aPtr[1] ^ 0x0; rPtr[2] = aPtr[2] ^ 0x0; rPtr[3] = aPtr[3] ^ 0x0; return r; }
inline __vec2lf __vnot2lf(const __vec2lf a) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); rPtr[0] = aPtr[0] ^ 0x0; rPtr[1] = aPtr[1] ^ 0x0; rPtr[2] = aPtr[2] ^ 0x0; rPtr[3] = aPtr[3] ^ 0x0; return r; }

/***** Or *****/
inline  __vec4i  __vor4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] | bPtr[0]; rPtr[1] = aPtr[1] | bPtr[1]; rPtr[2] = aPtr[2] | bPtr[2]; rPtr[3] = aPtr[3] | bPtr[3]; return r; }
inline  __vec4f  __vor4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] | bPtr[0]; rPtr[1] = aPtr[1] | bPtr[1]; rPtr[2] = aPtr[2] | bPtr[2]; rPtr[3] = aPtr[3] | bPtr[3]; return r; }
inline __vec2lf __vor2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] | bPtr[0]; rPtr[1] = aPtr[1] | bPtr[1]; rPtr[2] = aPtr[2] | bPtr[2]; rPtr[3] = aPtr[3] | bPtr[3]; return r; }

/***** Nor *****/
inline  __vec4i  __vnor4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] | bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] | bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] | bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] | bPtr[3]) ^ 0x0; return r; }
inline  __vec4f  __vnor4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] | bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] | bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] | bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] | bPtr[3]) ^ 0x0; return r; }
inline __vec2lf __vnor2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] | bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] | bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] | bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] | bPtr[3]) ^ 0x0; return r; }

/***** And *****/
inline  __vec4i  __vand4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] & bPtr[0]; rPtr[1] = aPtr[1] & bPtr[1]; rPtr[2] = aPtr[2] & bPtr[2]; rPtr[3] = aPtr[3] & bPtr[3]; return r; }
inline  __vec4f  __vand4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] & bPtr[0]; rPtr[1] = aPtr[1] & bPtr[1]; rPtr[2] = aPtr[2] & bPtr[2]; rPtr[3] = aPtr[3] & bPtr[3]; return r; }
inline __vec2lf __vand2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] & bPtr[0]; rPtr[1] = aPtr[1] & bPtr[1]; rPtr[2] = aPtr[2] & bPtr[2]; rPtr[3] = aPtr[3] & bPtr[3]; return r; }

/***** Nand *****/
inline  __vec4i  __vnand4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] & bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] & bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] & bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] & bPtr[3]) ^ 0x0; return r; }
inline  __vec4f  __vnand4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] & bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] & bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] & bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] & bPtr[3]) ^ 0x0; return r; }
inline __vec2lf __vnand2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] & bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] & bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] & bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] & bPtr[3]) ^ 0x0; return r; }

/***** Xor *****/
inline  __vec4i  __vxor4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] ^ bPtr[0]; rPtr[1] = aPtr[1] ^ bPtr[1]; rPtr[2] = aPtr[2] ^ bPtr[2]; rPtr[3] = aPtr[3] ^ bPtr[3]; return r; }
inline  __vec4f  __vxor4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] ^ bPtr[0]; rPtr[1] = aPtr[1] ^ bPtr[1]; rPtr[2] = aPtr[2] ^ bPtr[2]; rPtr[3] = aPtr[3] ^ bPtr[3]; return r; }
inline __vec2lf __vxor2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = aPtr[0] ^ bPtr[0]; rPtr[1] = aPtr[1] ^ bPtr[1]; rPtr[2] = aPtr[2] ^ bPtr[2]; rPtr[3] = aPtr[3] ^ bPtr[3]; return r; }

/***** Nxor *****/
inline  __vec4i  __vnxor4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] ^ bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] ^ bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] ^ bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] ^ bPtr[3]) ^ 0x0; return r; }
inline  __vec4f  __vnxor4f(const  __vec4f a, const  __vec4f b) {  __vec4f r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] ^ bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] ^ bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] ^ bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] ^ bPtr[3]) ^ 0x0; return r; }
inline __vec2lf __vnxor2lf(const __vec2lf a, const __vec2lf b) { __vec2lf r; int* rPtr = (int*)(&r); int* aPtr = (int*)(&a); int* bPtr = (int*)(&b); rPtr[0] = (aPtr[0] ^ bPtr[0]) ^ 0x0; rPtr[1] = (aPtr[1] ^ bPtr[1]) ^ 0x0; rPtr[2] = (aPtr[2] ^ bPtr[2]) ^ 0x0; rPtr[3] = (aPtr[3] ^ bPtr[3]) ^ 0x0; return r; }

/* TODO | FIXME - Try to do the comparisons in a branchless way */

/***** Equal To *****/
inline __vec4i  __vcmpeq4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = ((a.v0 == b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 == b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 == b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 == b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i  __vcmpeq4f(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = ((a.v0 == b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 == b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 == b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 == b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i __vcmpeq2lf(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = r.v1 = ((a.v0 == b.v0) ? (0xFFFFFFFF) : (0x0)); r.v2 = r.v3 = ((a.v1 == b.v1) ? (0xFFFFFFFF) : (0x0)); return r; }

/***** Greater Than *****/
inline __vec4i  __vcmpgt4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = ((a.v0 > b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 > b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 > b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 > b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i  __vcmpgt4f(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = ((a.v0 > b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 > b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 > b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 > b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i __vcmpgt2lf(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = r.v1 = ((a.v0 > b.v0) ? (0xFFFFFFFF) : (0x0)); r.v2 = r.v3 = ((a.v1 > b.v1) ? (0xFFFFFFFF) : (0x0)); return r; }

/***** Greater Than Or Equal To *****/
inline __vec4i  __vcmpge4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = ((a.v0 >= b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 >= b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 >= b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 >= b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i  __vcmpge4f(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = ((a.v0 >= b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 >= b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 >= b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 >= b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i __vcmpge2lf(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = r.v1 = ((a.v0 >= b.v0) ? (0xFFFFFFFF) : (0x0)); r.v2 = r.v3 = ((a.v1 >= b.v1) ? (0xFFFFFFFF) : (0x0)); return r; }

/***** Less Than *****/
inline __vec4i  __vcmplt4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = ((a.v0 < b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 < b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 < b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 < b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i  __vcmplt4f(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = ((a.v0 < b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 < b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 < b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 < b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i __vcmplt2lf(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = r.v1 = ((a.v0 < b.v0) ? (0xFFFFFFFF) : (0x0)); r.v2 = r.v3 = ((a.v1 < b.v1) ? (0xFFFFFFFF) : (0x0)); return r; }

/***** Less Than Or Equal To *****/
inline __vec4i  __vcmple4i(const  __vec4i a, const  __vec4i b) {  __vec4i r; r.v0 = ((a.v0 <= b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 <= b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 <= b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 <= b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i  __vcmple4f(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = ((a.v0 <= b.v0) ? (0xFFFFFFFF) : (0x0)); r.v1 = ((a.v1 <= b.v1) ? (0xFFFFFFFF) : (0x0)); r.v2 = ((a.v2 <= b.v2) ? (0xFFFFFFFF) : (0x0)); r.v3 = ((a.v3 <= b.v3) ? (0xFFFFFFFF) : (0x0)); return r; }
inline __vec4i __vcmple2lf(const  __vec4f a, const  __vec4f b) {  __vec4i r; r.v0 = r.v1 = ((a.v0 <= b.v0) ? (0xFFFFFFFF) : (0x0)); r.v2 = r.v3 = ((a.v1 <= b.v1) ? (0xFFFFFFFF) : (0x0)); return r; }


/*******************************************************************************
 ***** C++ Operators for Generic Implementation
 *******************************************************************************/
#if defined(__cplusplus)

  /***** Addition *****/
  inline  __vec4i operator+(const  __vec4i &a, const  __vec4i &b) { return  __vadd4i(a, b); }
  inline  __vec4f operator+(const  __vec4f &a, const  __vec4f &b) { return  __vadd4f(a, b); }
  inline __vec2lf operator+(const __vec2lf &a, const __vec2lf &b) { return __vadd2lf(a, b); }
  inline  __vec4i operator+=( __vec4i &a, const  __vec4i &b) { a =  __vadd4i(a, b); return a; }
  inline  __vec4f operator+=( __vec4f &a, const  __vec4f &b) { a =  __vadd4f(a, b); return a; }
  inline __vec2lf operator+=(__vec2lf &a, const __vec2lf &b) { a = __vadd2lf(a, b); return a; }

  inline  __vec4i operator+(const  __vec4i &a, const    int &b) { return  __vadd4i(a,  __vset4i(b)); }
  inline  __vec4f operator+(const  __vec4f &a, const  float &b) { return  __vadd4f(a,  __vset4f(b)); }
  inline __vec2lf operator+(const __vec2lf &a, const double &b) { return __vadd2lf(a, __vset2lf(b)); }
  inline  __vec4i operator+=( __vec4i &a, const    int &b) { a =  __vadd4i(a,  __vset4i(b)); return a; }
  inline  __vec4f operator+=( __vec4f &a, const  float &b) { a =  __vadd4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator+=(__vec2lf &a, const double &b) { a = __vadd2lf(a, __vset2lf(b)); return a; }

  /***** Subtraction *****/
  inline  __vec4i operator-(const  __vec4i &a, const  __vec4i &b) { return  __vsub4i(a, b); }
  inline  __vec4f operator-(const  __vec4f &a, const  __vec4f &b) { return  __vsub4f(a, b); }
  inline __vec2lf operator-(const __vec2lf &a, const __vec2lf &b) { return __vsub2lf(a, b); }
  inline  __vec4i operator-=( __vec4i &a, const  __vec4i &b) { a =  __vsub4i(a, b); return a; }
  inline  __vec4f operator-=( __vec4f &a, const  __vec4f &b) { a =  __vsub4f(a, b); return a; }
  inline __vec2lf operator-=(__vec2lf &a, const __vec2lf &b) { a = __vsub2lf(a, b); return a; }

  inline  __vec4i operator-(const  __vec4i &a, const    int &b) { return  __vsub4i(a,  __vset4i(b)); }
  inline  __vec4f operator-(const  __vec4f &a, const  float &b) { return  __vsub4f(a,  __vset4f(b)); }
  inline __vec2lf operator-(const __vec2lf &a, const double &b) { return __vsub2lf(a, __vset2lf(b)); }
  inline  __vec4i operator-=( __vec4i &a, const    int &b) { a =  __vsub4i(a,  __vset4i(b)); return a; }
  inline  __vec4f operator-=( __vec4f &a, const  float &b) { a =  __vsub4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator-=(__vec2lf &a, const double &b) { a = __vsub2lf(a, __vset2lf(b)); return a; }

  /***** Multiplication *****/
  inline  __vec4f operator*(const  __vec4f &a, const  __vec4f &b) { return  __vmul4f(a, b); }
  inline __vec2lf operator*(const __vec2lf &a, const __vec2lf &b) { return __vmul2lf(a, b); }
  inline  __vec4f operator*=( __vec4f &a, const  __vec4f &b) { a =  __vmul4f(a, b); return a; }
  inline __vec2lf operator*=(__vec2lf &a, const __vec2lf &b) { a = __vmul2lf(a, b); return a; }

  inline  __vec4f operator*(const  __vec4f &a, const  float &b) { return  __vmul4f(a,  __vset4f(b)); }
  inline __vec2lf operator*(const __vec2lf &a, const double &b) { return __vmul2lf(a, __vset2lf(b)); }
  inline  __vec4f operator*=( __vec4f &a, const  float &b) { a =  __vmul4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator*=(__vec2lf &a, const double &b) { a = __vmul2lf(a, __vset2lf(b)); return a; }

  /***** Division *****/
  inline  __vec4f operator/(const  __vec4f &a, const  __vec4f &b) { return  __vdiv4f(a, b); }
  inline __vec2lf operator/(const __vec2lf &a, const __vec2lf &b) { return __vdiv2lf(a, b); }
  inline  __vec4f operator/=( __vec4f &a, const  __vec4f &b) { a =  __vdiv4f(a, b); return a; }
  inline __vec2lf operator/=(__vec2lf &a, const __vec2lf &b) { a = __vdiv2lf(a, b); return a; }

  inline  __vec4f operator/(const  __vec4f &a, const  float &b) { return  __vdiv4f(a,  __vset4f(b)); }
  inline __vec2lf operator/(const __vec2lf &a, const double &b) { return __vdiv2lf(a, __vset2lf(b)); }
  inline  __vec4f operator/=( __vec4f &a, const  float &b) { a =  __vdiv4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator/=(__vec2lf &a, const double &b) { a = __vdiv2lf(a, __vset2lf(b)); return a; }

  /***** Or *****/
  inline  __vec4i operator|(const  __vec4i &a, const  __vec4i &b) { return  __vor4i(a, b); }
  inline  __vec4f operator|(const  __vec4f &a, const  __vec4f &b) { return  __vor4f(a, b); }
  inline __vec2lf operator|(const __vec2lf &a, const __vec2lf &b) { return __vor2lf(a, b); }
  inline  __vec4i operator|=( __vec4i &a, const  __vec4i &b) { a =  __vor4i(a, b); return a; }
  inline  __vec4f operator|=( __vec4f &a, const  __vec4f &b) { a =  __vor4f(a, b); return a; }
  inline __vec2lf operator|=(__vec2lf &a, const __vec2lf &b) { a = __vor2lf(a, b); return a; }

  inline  __vec4i operator|(const  __vec4i &a, const    int &b) { return  __vor4i(a,  __vset4i(b)); }
  inline  __vec4f operator|(const  __vec4f &a, const  float &b) { return  __vor4f(a,  __vset4f(b)); }
  inline __vec2lf operator|(const __vec2lf &a, const double &b) { return __vor2lf(a, __vset2lf(b)); }
  inline  __vec4i operator|=( __vec4i &a, const    int &b) { a =  __vor4i(a,  __vset4i(b)); return a; }
  inline  __vec4f operator|=( __vec4f &a, const  float &b) { a =  __vor4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator|=(__vec2lf &a, const double &b) { a = __vor2lf(a, __vset2lf(b)); return a; }

  /***** And *****/
  inline  __vec4i operator&(const  __vec4i &a, const  __vec4i &b) { return  __vand4i(a, b); }
  inline  __vec4f operator&(const  __vec4f &a, const  __vec4f &b) { return  __vand4f(a, b); }
  inline __vec2lf operator&(const __vec2lf &a, const __vec2lf &b) { return __vand2lf(a, b); }
  inline  __vec4i operator&=( __vec4i &a, const  __vec4i &b) { a =  __vand4i(a, b); return a; }
  inline  __vec4f operator&=( __vec4f &a, const  __vec4f &b) { a =  __vand4f(a, b); return a; }
  inline __vec2lf operator&=(__vec2lf &a, const __vec2lf &b) { a = __vand2lf(a, b); return a; }

  inline  __vec4i operator&(const  __vec4i &a, const    int &b) { return  __vand4i(a,  __vset4i(b)); }
  inline  __vec4f operator&(const  __vec4f &a, const  float &b) { return  __vand4f(a,  __vset4f(b)); }
  inline __vec2lf operator&(const __vec2lf &a, const double &b) { return __vand2lf(a, __vset2lf(b)); }
  inline  __vec4i operator&=( __vec4i &a, const    int &b) { a =  __vand4i(a,  __vset4i(b)); return a; }
  inline  __vec4f operator&=( __vec4f &a, const  float &b) { a =  __vand4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator&=(__vec2lf &a, const double &b) { a = __vand2lf(a, __vset2lf(b)); return a; }

  /***** Xor *****/
  inline  __vec4i operator^(const  __vec4i &a, const  __vec4i &b) { return  __vxor4i(a, b); }
  inline  __vec4f operator^(const  __vec4f &a, const  __vec4f &b) { return  __vxor4f(a, b); }
  inline __vec2lf operator^(const __vec2lf &a, const __vec2lf &b) { return __vxor2lf(a, b); }
  inline  __vec4i operator^=( __vec4i &a, const  __vec4i &b) { a =  __vxor4i(a, b); return a; }
  inline  __vec4f operator^=( __vec4f &a, const  __vec4f &b) { a =  __vxor4f(a, b); return a; }
  inline __vec2lf operator^=(__vec2lf &a, const __vec2lf &b) { a = __vxor2lf(a, b); return a; }

  inline  __vec4i operator^(const  __vec4i &a, const    int &b) { return  __vxor4i(a,  __vset4i(b)); }
  inline  __vec4f operator^(const  __vec4f &a, const  float &b) { return  __vxor4f(a,  __vset4f(b)); }
  inline __vec2lf operator^(const __vec2lf &a, const double &b) { return __vxor2lf(a, __vset2lf(b)); }
  inline  __vec4i operator^=( __vec4i &a, const    int &b) { a =  __vxor4i(a,  __vset4i(b)); return a; }
  inline  __vec4f operator^=( __vec4f &a, const  float &b) { a =  __vxor4f(a,  __vset4f(b)); return a; }
  inline __vec2lf operator^=(__vec2lf &a, const double &b) { a = __vxor2lf(a, __vset2lf(b)); return a; }

#endif /* defined(__cplusplus) */

/*@}*/


// NOTE: Wrap the rest of the file in an 'else' portion of a '#if' so doxygen
// ignores it (this is a complete hack, but works for now until a better
// solution can be found).
#if defined(__DOXYGEN_IGNORE_ALL_THIS__)


/*******************************************************************************
 *******************************************************************************
 ***** SSE Support
 *******************************************************************************
 *******************************************************************************/
#elif defined(__SSE2__) && (!(FORCE_NO_SSE))

  /* NOTE | TODO | FIXME : Add checks for various version of SSE.  For now, only
   *   support and assume that minimum level SSE2.
   */

  /***** Data Types *****/
  typedef __m128i  vec4i;
  typedef  __m128  vec4f;
  typedef __m128d vec2lf;

  /***** Insert *****/
  /* TODO | FIXME - Try to make these functions not reference memory so values stay in registers */
  inline  vec4i  vinsert4i( vec4i v, const    int s, const int i) {  vec4i r = v;    int* rPtr = (   int*)(&r); rPtr[i] = s; return r; }
  inline  vec4f  vinsert4f( vec4f v, const  float s, const int i) {  vec4f r = v;  float* rPtr = ( float*)(&r); rPtr[i] = s; return r; }
  inline vec2lf vinsert2lf(vec2lf v, const double s, const int i) { vec2lf r = v; double* rPtr = (double*)(&r); rPtr[i] = s; return r; }

  /***** Extract *****/
  /* TODO | FIXME - Try to make these functions not reference memory so values stay in registers */
  inline    int  vextract4i( vec4i v, const int i) { return ((   int*)(&v))[i]; }
  inline  float  vextract4f( vec4f v, const int i) { return (( float*)(&v))[i]; }
  inline double vextract2lf(vec2lf v, const int i) { return ((double*)(&v))[i]; }

  /***** Set *****/
  #define  vset4i(a)  (_mm_set1_epi32((int)(a)))
  #define  vset4f(a)  (_mm_set1_ps((float)(a)))
  #define vset2lf(a)  (_mm_set1_pd((double)(a)))

  /***** Constant Zero *****/
  #define  const_vzero4i  (_mm_setzero_si128())
  #define  const_vzero4f  (_mm_setzero_ps())
  #define const_vzero2lf  (_mm_setzero_pd())

  /***** Constant One *****/
  #define  const_vone4i  (vset4i(1))
  #define  const_vone4f  (vset4f(1.0f))
  #define const_vone2lf  (vset2lf(1.0))

  /***** Constant Two *****/
  #define  const_vtwo4i  (vset4i(2))
  #define  const_vtwo4f  (vset4f(2.0f))
  #define const_vtwo2lf  (vset2lf(2.0))

  /***** Constant Negative One *****/
  #define  const_vnegone4i  (vset4i(-1))
  #define  const_vnegone4f  (vset4f(-1.0f))
  #define const_vnegone2lf  (vset2lf(-1.0))

  /***** Rotate *****/
  /* TODO : FIXME - Find a better way to do Rotate in SSE */
  inline  vec4i  vroth4i(const  vec4i &a, int s) {  vec4i b;    int* a_ptr = (   int*)(&a);    int* b_ptr = (   int*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0-s)&0x3]; b_ptr[1] = a_ptr[(1-s)&0x3]; b_ptr[2] = a_ptr[(2-s)&0x3]; b_ptr[3] = a_ptr[(3-s)&0x3]; return b; }
  inline  vec4f  vroth4f(const  vec4f &a, int s) {  vec4f b;  float* a_ptr = ( float*)(&a);  float* b_ptr = ( float*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0-s)&0x3]; b_ptr[1] = a_ptr[(1-s)&0x3]; b_ptr[2] = a_ptr[(2-s)&0x3]; b_ptr[3] = a_ptr[(3-s)&0x3]; return b; }
  inline vec2lf vroth2lf(const vec2lf &a, int s) { vec2lf b; double* a_ptr = (double*)(&a); double* b_ptr = (double*)(&b); s &= 0x1; b_ptr[0] = a_ptr[(0-s)&0x1]; b_ptr[1] = a_ptr[(1-s)&0x1]; return b; }
  inline  vec4i  vrotl4i(const  vec4i &a, int s) {  vec4i b;    int* a_ptr = (   int*)(&a);    int* b_ptr = (   int*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0+s)&0x3]; b_ptr[1] = a_ptr[(1+s)&0x3]; b_ptr[2] = a_ptr[(2+s)&0x3]; b_ptr[3] = a_ptr[(3+s)&0x3]; return b; }
  inline  vec4f  vrotl4f(const  vec4f &a, int s) {  vec4f b;  float* a_ptr = ( float*)(&a);  float* b_ptr = ( float*)(&b); s &= 0x3; b_ptr[0] = a_ptr[(0+s)&0x3]; b_ptr[1] = a_ptr[(1+s)&0x3]; b_ptr[2] = a_ptr[(2+s)&0x3]; b_ptr[3] = a_ptr[(3+s)&0x3]; return b; }
  inline vec2lf vrotl2lf(const vec2lf &a, int s) { vec2lf b; double* a_ptr = (double*)(&a); double* b_ptr = (double*)(&b); s &= 0x1; b_ptr[0] = a_ptr[(0+s)&0x1]; b_ptr[1] = a_ptr[(1+s)&0x1]; return b; }

  /***** Addition *****/
  #define  vadd4i(a, b)  (_mm_add_epi32((a), (b)))
  #define  vadd4f(a, b)  (_mm_add_ps((a), (b)))
  #define vadd2lf(a, b)  (_mm_add_pd((a), (b)))

  /***** Subtraction *****/
  #define  vsub4i(a, b)  (_mm_sub_epi32((a), (b)))
  #define  vsub4f(a, b)  (_mm_sub_ps((a), (b)))
  #define vsub2lf(a, b)  (_mm_sub_pd((a), (b)))

  /***** Multiplication *****/
  #define    vmul4f(a, b)  (_mm_mul_ps((a), (b)))
  #define   vmul2lf(a, b)  (_mm_mul_pd((a), (b)))

  /***** Division *****/
  #define   vdiv4f(a, b)  (_mm_div_ps((a), (b)))
  #define  vdiv2lf(a, b)  (_mm_div_pd((a), (b)))

  /***** Fused Multiply Add *****/
  #define  vmadd4f(a, b, c)  ( vadd4f( vmul4f((a), (b)), (c)))
  #define vmadd2lf(a, b, c)  (vadd2lf(vmul2lf((a), (b)), (c)))

  /***** Reciprocal *****/
  #define  vrecip4f(a)  (_mm_rcp_ps(a))
  #define vrecip2lf(a)  (_mm_rcp_pd(a))

  /***** Square Root *****/
  #define  vsqrt4f(a)  (_mm_sqrt_ps(a))
  #define vsqrt2lf(a)  (_mm_sqrt_pd(a))

  /***** Reciprocal Square Root *****/
  #define  vrsqrt4f(a)  (_mm_rsqrt_ps(a))
  #define vrsqrt2lf(a)  (vrecip2lf(vsqrt2lf(a)))

  /***** Not *****/
  #define  vnot4i(a)  (_mm_xor_si128((a), const_vzero4i))
  #define  vnot4f(a)  (_mm_xor_ps((a), const_vzero4f))
  #define vnot2lf(a)  (_mm_xor_pd((a), const_vzero2lf))

  /***** Or *****/
  #define  vor4i(a, b)  (_mm_or_si128((a), (b)))
  #define  vor4f(a, b)  (_mm_or_ps((a), (b)))
  #define vor2lf(a, b)  (_mm_or_pd((a), (b)))

  /***** Nor *****/
  #define  vnor4i(a, b)  ( vnot4i( vor4i((a), (b))))
  #define  vnor4f(a, b)  ( vnot4f( vor4f((a), (b))))
  #define vnor2lf(a, b)  (vnot2lf(vor2lf((a), (b))))

  /***** And *****/
  #define  vand4i(a, b)  (_mm_and_si128((a), (b)))
  #define  vand4f(a, b)  (_mm_and_ps((a), (b)))
  #define vand2lf(a, b)  (_mm_and_pd((a), (b)))

  /***** Nand *****/
  #define  vnand4i(a, b)  ( vnot4i( vand4i((a), (b))))
  #define  vnand4f(a, b)  ( vnot4f( vand4f((a), (b))))
  #define vnand2lf(a, b)  (vnot2lf(vand2lf((a), (b))))

  /***** Xor *****/
  #define  vxor4i(a, b)  (_mm_xor_si128((a), (b)))
  #define  vxor4f(a, b)  (_mm_xor_ps((a), (b)))
  #define vxor2lf(a, b)  (_mm_xor_pd((a), (b)))

  /***** Nxor *****/
  #define  vnxor4i(a, b)  ( vnot4i( vxor4i((a), (b))))
  #define  vnxor4f(a, b)  ( vnot4f( vxor4f((a), (b))))
  #define vnxor2lf(a, b)  (vnot2lf(vxor2lf((a), (b))))

  /***** Equal To *****/
  #define  vcmpeq4i(a, b)  ((_m128i)(_mm_cmpeq_epi32((a), (b))))
  #define  vcmpeq4f(a, b)  ((_m128i)(_mm_cmpeq_ps((a), (b))))
  #define vcmpeq2lf(a, b)  ((_m128i)(_mm_cmpeq_pd((a), (b))))

  /***** Greater Than *****/
  #define  vcmpgt4i(a, b)  ((_m128i)(_mm_cmpgt_epi32((a), (b))))
  #define  vcmpgt4f(a, b)  ((_m128i)(_mm_cmpgt_ps((a), (b))))
  #define vcmpgt2lf(a, b)  ((_m128i)(_mm_cmpgt_pd((a), (b))))

  /***** Greater Than Or Equal To *****/
  #define  vcmpge4i(a, b)  ((_m128i)(_mm_cmpge_epi32((a), (b))))
  #define  vcmpge4f(a, b)  ((_m128i)(_mm_cmpge_ps((a), (b))))
  #define vcmpge2lf(a, b)  ((_m128i)(_mm_cmpge_pd((a), (b))))

  /***** Less Than *****/
  #define  vcmplt4i(a, b)  ((_m128i)(_mm_cmplt_epi32((a), (b))))
  #define  vcmplt4f(a, b)  ((_m128i)(_mm_cmplt_ps((a), (b))))
  #define vcmplt2lf(a, b)  ((_m128i)(_mm_cmplt_pd((a), (b))))

  /***** Less Than Or Equal To *****/
  #define  vcmple4i(a, b)  ((_m128i)(_mm_cmple_epi32((a), (b))))
  #define  vcmple4f(a, b)  ((_m128i)(_mm_cmple_ps((a), (b))))
  #define vcmple2lf(a, b)  ((_m128i)(_mm_cmple_pd((a), (b))))


/*******************************************************************************
 *******************************************************************************
 ***** SPE SIMD Instructions
 *******************************************************************************
 *******************************************************************************/
/* TODO | FIXME : Find a more general check for this (this is Charm++ specific) */
#elif (CMK_CELL_SPE != 0) && (!(FORCE_NO_SPE_SIMD))

  /***** Data Types *****/
  typedef vector signed int vec4i;
  typedef vector float vec4f;
  typedef vector double vec2lf;

  /***** Insert *****/
  #define  vinsert4i(v, s, i)  (spu_insert((s), (v), (i)))
  #define  vinsert4f(v, s, i)  (spu_insert((s), (v), (i)))
  #define vinsert2lf(v, s, i)  (spu_insert((s), (v), (i)))

  /***** Extract *****/
  #define  vextract4i(v, i)  (spu_extract((v), (i)))
  #define  vextract4f(v, i)  (spu_extract((v), (i)))
  #define vextract2lf(v, i)  (spu_extract((v), (i)))

  /***** Set *****/
  #define  vset4i(a)  (spu_splats((int)(a)))
  #define  vset4f(a)  (spu_splats((float)(a)))
  #define vset2lf(a)  (spu_splats((double)(a)))

  /***** Constant Zero *****/
  #define  const_vzero4i  (vset4i(0))
  #define  const_vzero4f  (vset4f(0.0f))
  #define const_vzero2lf  (vset2lf(0.0))

  /***** Constant One *****/
  #define  const_vone4i  (vset4i(1))
  #define  const_vone4f  (vset4f(1.0f))
  #define const_vone2lf  (vset2lf(1.0))

  /***** Constant Two *****/
  #define  const_vtwo4i  (vset4i(2))
  #define  const_vtwo4f  (vset4f(2.0f))
  #define const_vtwo2lf  (vset2lf(2.0))

  /***** Constant Negative One *****/
  #define  const_vnegone4i  (vset4i(-1))
  #define  const_vnegone4f  (vset4f(-1.0f))
  #define const_vnegone2lf  (vset2lf(-1.0))

  /***** Rotate *****/
  #define   vroth4i(a, s) (spu_rlqwbyte((a), (0x10-(((s)&0x3)<<2)) ))
  #define   vroth4f(a, s) (spu_rlqwbyte((a), (0x10-(((s)&0x3)<<2)) ))
  #define  vroth2lf(a, s) (spu_rlqwbyte((a),       (((s)&0x1)<<3)  ))
  #define   vrotl4i(a, s) (spu_rlqwbyte((a), ((s)&0x3)<<2))
  #define   vrotl4f(a, s) (spu_rlqwbyte((a), ((s)&0x3)<<2))
  #define  vrotl2lf(a, s) (spu_rlqwbyte((a), ((s)&0x1)<<3))

  /***** Addition *****/
  #define  vadd4i(a, b)  (spu_add((a), (b)))
  #define  vadd4f(a, b)  (spu_add((a), (b)))
  #define vadd2lf(a, b)  (spu_add((a), (b)))

  /***** Subtraction *****/
  #define  vsub4i(a, b)  (spu_sub((a), (b)))
  #define  vsub4f(a, b)  (spu_sub((a), (b)))
  #define vsub2lf(a, b)  (spu_sub((a), (b)))

  /***** Multiplication *****/
  #define   vmul4f(a, b)  (spu_mul((a), (b)))
  #define  vmul2lf(a, b)  (spu_mul((a), (b)))

  /***** Division *****/
  #define vdiv4f(a, b)  (spu_mul((a), spu_re(b)))
  inline vec2lf vdiv2lf(const vec2lf a, const vec2lf b) { vec2lf r = { 0.0, 0.0 }; spu_insert((spu_extract(a, 0) / spu_extract(b, 0)), r, 0); spu_insert((spu_extract(a, 1) / spu_extract(b, 1)), r, 1); return r; }

  /***** Fused Multiply Add *****/
  #define  vmadd4f(a, b, c)  (spu_madd((a), (b), (c)))
  #define vmadd2lf(a, b, c)  (spu_madd((a), (b), (c)))

  /***** Reciprocal *****/
  #define  vrecip4f(a)  (spu_re(a))
  inline vec2lf vrecip2lf(const vec2lf a, const vec2lf b) { vec2lf r = { 0.0, 0.0 }; spu_insert((1.0f / spu_extract(a, 0)), r, 0); spu_insert((1.0f / spu_extract(a, 1)), r, 1); return r; }

  /***** Square Root *****/
  #define vsqrt4f(a) (spu_re(spu_rsqrte(a)))
  inline vec2lf vsqrt2lf(const vec2lf a, const vec2lf b) { vec2lf r = { 0.0, 0.0 }; spu_insert(sqrt(spu_extract(a, 0)), r, 0); spu_insert(sqrt(spu_extract(a, 1)), r, 1); return r; }

  /***** Reciprocal Square Root *****/
  #define vrsqrt4f(a) (spu_rsqrte(a))
  inline vec2lf vrsqrt2lf(const vec2lf a, const vec2lf b) { vec2lf r = { 0.0, 0.0 }; spu_insert((1.0f / sqrt(spu_extract(a, 0))), r, 0); spu_insert((1.0f / sqrt(spu_extract(a, 1))), r, 1); return r; }

  /***** Not *****/
  #define  vnot4i(a)  (spu_nor((a), (a)))
  #define  vnot4f(a)  (spu_nor((a), (a)))
  #define vnot2lf(a)  (spu_nor((a), (a)))

  /***** Or *****/
  #define  vor4i(a, b)  (spu_or((a), (b)))
  #define  vor4f(a, b)  (spu_or((a), (b)))
  #define vor2lf(a, b)  (spu_or((a), (b)))

  /***** Nor *****/
  #define  vnor4i(a, b)  (spu_nor((a), (b)))
  #define  vnor4f(a, b)  (spu_nor((a), (b)))
  #define vnor2lf(a, b)  (spu_nor((a), (b)))

  /***** And *****/
  #define  vand4i(a, b)  (spu_and((a), (b)))
  #define  vand4f(a, b)  (spu_and((a), (b)))
  #define vand2lf(a, b)  (spu_and((a), (b)))

  /***** Nand *****/
  #define  vnand4i(a, b)  (spu_nand((a), (b)))
  #define  vnand4f(a, b)  (spu_nand((a), (b)))
  #define vnand2lf(a, b)  (spu_nand((a), (b)))

  /***** Xor *****/
  #define  vxor4i(a, b)  (spu_xor((a), (b)))
  #define  vxor4f(a, b)  (spu_xor((a), (b)))
  #define vxor2lf(a, b)  (spu_xor((a), (b)))

  /***** Nxor *****/
  #define  vnxor4i(a, b)  ( vnot4i( vxor4i((a), (b))))
  #define  vnxor4f(a, b)  ( vnot4f( vxor4f((a), (b))))
  #define vnxor2lf(a, b)  (vnot2lf(vxor2lf((a), (b))))

  /***** Equal To *****/
  #define  vcmpeq4i(a, b)  ((vec4i)(spu_cmpeq((a), (b))))
  #define  vcmpeq4f(a, b)  ((vec4i)(spu_cmpeq((a), (b))))
  #define vcmpeq2lf(a, b)  ((vec4i)(spu_cmpeq((a), (b))))

  /***** Greater Than *****/
  #define  vcmpgt4i(a, b)  ((vec4i)(spu_cmpgt((a), (b))))
  #define  vcmpgt4f(a, b)  ((vec4i)(spu_cmpgt((a), (b))))
  #define vcmpgt2lf(a, b)  ((vec4i)(spu_cmpgt((a), (b))))

  /***** Greater Than or Equal To *****/
  #define  vcmpge4i(a, b)  (spu_or( vcmpeq4i((a), (b)),  vcmpgt4i((a), (b))))
  #define  vcmpge4f(a, b)  (spu_or( vcmpeq4f((a), (b)),  vcmpgt4f((a), (b))))
  #define vcmpge2lf(a, b)  (spu_or(vcmpeq2lf((a), (b)), vcmpgt2lf((a), (b))))

  /***** Less Than *****/
  #define  vcmplt4i(a, b)  (spu_nor( vcmpgt4i((a), (b)),  vcmpeq4i((a), (b))))
  #define  vcmplt4f(a, b)  (spu_nor( vcmpgt4f((a), (b)),  vcmpeq4f((a), (b))))
  #define vcmplt2lf(a, b)  (spu_nor(vcmpgt2lf((a), (b)), vcmpeq2lf((a), (b))))

  /***** Less Than or Equal To *****/
  #define  vcmple4i(a, b)  (spu_nor( vcmpgt4i((a), (b)),  vxor4i((a), (a))))
  #define  vcmple4f(a, b)  (spu_nor( vcmpgt4f((a), (b)),  vxor4f((a), (a))))
  #define vcmple2lf(a, b)  (spu_nor(vcmpgt2lf((a), (b)), vxor2lf((a), (b))))


/*******************************************************************************
 *******************************************************************************
 ***** AltiVec
 *******************************************************************************
 *******************************************************************************/
#elif defined(__VEC__) && (!(FORCE_NO_ALTIVEC))

  /***** Data Types *****/
  typedef vector signed int vec4i;
  typedef vector float vec4f;
  typedef __vec2lf vec2lf;

  /***** Insert *****/
  /* TODO | FIXME - Try to make these functions not reference memory so values stay in registers */
  inline  vec4i  vinsert4i( vec4i v, const    int s, const int i) {  vec4i r = v;    int* rPtr = (   int*)(&r); rPtr[i] = s; return r; }
  inline  vec4f  vinsert4f( vec4f v, const  float s, const int i) {  vec4f r = v;  float* rPtr = ( float*)(&r); rPtr[i] = s; return r; }
  inline vec2lf vinsert2lf(vec2lf v, const double s, const int i) { vec2lf r = v; double* rPtr = (double*)(&r); rPtr[i] = s; return r; }

  /***** Extract *****/
  /* TODO | FIXME - Try to make these functions not reference memory so values stay in registers */
  inline    int  vextract4i( vec4i v, const int i) {    int* vPtr = (   int*)(&v); return vPtr[i]; }
  inline  float  vextract4f( vec4f v, const int i) {  float* vPtr = ( float*)(&v); return vPtr[i]; }
  inline double vextract2lf(vec2lf v, const int i) { double* vPtr = (double*)(&v); return vPtr[i]; }

  /***** Set *****/
  /* TODO : FIXME - There must be a better way to do this, but it seems the
   *   only way to convert scalar to vector is to go through memory instructions.
   */
  inline vec4i vset4i(const   int a) { __vec4i r; r.v0 = a; return vec_splat(*((vec4i*)(&r)), 0); }
  inline vec4f vset4f(const float a) { __vec4f r; r.v0 = a; return vec_splat(*((vec4f*)(&r)), 0); }
  #define vset2lf __vset2lf

  /* NOTE: Declare one for unsigned char vector also (required by rotate functions) */
  inline vector unsigned char vset16uc(const unsigned char c) { vector unsigned char r __attribute__((aligned(16))); ((unsigned char*)(&r))[0] = c; return vec_splat(r, 0); }

  /***** Constant Zero *****/
  #define  const_vec4i  (vec_splat_s32(0))
  #define  const_vec4f  (vec_ctf(vec_splat_s32(0), 0))
  #define const_vec2lf  (__const_vzero2lf)

  /***** Constant One *****/
  #define  const_vec4i  (vec_splat_s32(1))
  #define  const_vec4f  (vec_ctf(vec_splat_s32(1), 0))
  #define const_vec2lf  (__const_vone2lf)

  /***** Constant Two *****/
  #define  const_vec4i  (vec_splat_s32(2))
  #define  const_vec4f  (vec_ctf(vec_splat_s32(2), 0))
  #define const_vec2lf  (__const_vtwo2lf)

  /***** Constant Negative One *****/
  #define  const_vec4i  (vec_splat_s32(-1))
  #define  const_vec4f  (vec_ctf(vec_splat_s32(-1), 0))
  #define const_vec2lf  (__const_vec2lf)

  /***** Rotate *****/
  #define __vrotlbytes(a, s)  (vec_or(vec_slo((a), vset16uc(((s) & 0xf) << 3)), vec_sro((a), set16uc((16 - ((s) & 0xf)) << 3))))
  #define __vrotrbytes(a, s)  (vec_or(vec_sro((a), vset16uc(((s) & 0xf) << 3)), vec_slo((a), set16uc((16 - ((s) & 0xf)) << 3))))
  #define  vrotl4i(a, s)  __vrotlbytes((a), ((s) << 2))
  #define  vrotl4f(a, s)  __vrotlbytes((a), ((s) << 2))
  #define vrotl2lf(a, s)  __vrotlbytes((a), ((s) << 3))
  #define  vroth4i(a, s)  __vrotrbytes((a), ((s) << 2))
  #define  vroth4f(a, s)  __vrotrbytes((a), ((s) << 2))
  #define vroth2lf(a, s)  __vrotrbytes((a), ((s) << 3))

  /***** Addition *****/
  #define  vadd4i(a, b)  (vec_add((a), (b)))
  #define  vadd4f(a, b)  (vec_add((a), (b)))
  #define vadd2lf __vadd2lf

  /***** Subtraction *****/
  #define  vsub4i(a, b)  (vec_sub((a), (b)))
  #define  vsub4f(a, b)  (vec_sub((a), (b)))
  #define vsub2lf __vsub2lf

  /***** Multiplication *****/
  #define  vmul4f(a, b)  (vec_madd((a), (b), vec_xor((a))))
  #define vmul2lf __vmul2lf

  /***** Division *****/
  #define vdiv4f(a, b)  (vmul4f((a), vec_re(b)))
  #define vdiv2lf __vdiv2lf

  /***** Fused Multiply Add *****/
  #define vmadd4f(a, b, c)  (vec_madd((a), (b), (c)))
  #define vmadd2lf __vmadd2lf

  /***** Reciprocal *****/
  #define vrecip4f(a)  (vec_re(a))
  #define vrecip2lf __vrecip2lf

  /***** Square Root *****/
  #define vsqrt4f(a)  (vec_re(vec_rsqrte(a)))
  #define vsqrt2lf __vsqrt2lf

  /***** Reciprocal Square Root *****/
  #define vrsqrt4f(a)  (vec_rsqrte(a))
  #define vrsqrt2lf __vrsqrt2lf

  /***** Not *****/
  #define vnot4i(a)  (vec_xor((a), (a)))
  #define vnot4f(a)  (vec_xor((a), (a))) 
  #define vnot2lf __vnot2lf

  /***** Or *****/
  #define vor4i(a, b)  (vec_or((a), (b)))
  #define vor4f(a, b)  (vec_or((a), (b)))
  #define vor2lf __vor2lf

  /***** Nor *****/
  #define vnor4i(a, b)  (vec_nor((a), (b)))
  #define vnor4f(a, b)  (vec_nor((a), (b)))
  #define vnor2lf __vnor2lf

  /***** And *****/
  #define vand4i(a, b)  (vec_and((a), (b)))
  #define vand4f(a, b)  (vec_and((a), (b)))
  #define vand2lf __vand2lf

  /***** Nand *****/
  #define vnand4i(a, b)  (vnot4i(vand4i((a), (b))))
  #define vnand4f(a, b)  (vnot4f(vand4f((a), (b))))
  #define vnand2lf __vnand2lf

  /***** Xor *****/
  #define vxor4i(a, b)  (vec_xor((a), (b)))
  #define vxor4f(a, b)  (vec_xor((a), (b)))
  #define vxor2lf __vxor2lf

  /***** Nxor *****/
  #define vnxor4i(a, b)  (vnot4i(vxor4i((a), (b))))
  #define vnxor4f(a, b)  (vnot4f(vxor4f((a), (b))))
  #define vnxor2lf __vnxor2lf

  /***** Equal To *****/
  #define  vcmpeq4i(a, b)  ((vec4i)(vec_cmpeq((a), (b))))
  #define  vcmpeq4f(a, b)  ((vec4i)(vec_cmpeq((a), (b))))
  #define vcmpeq2lf __vcmpeq2lf

  /***** Greater Than *****/
  #define  vcmpgt4i(a, b)  ((vec4i)(vec_cmpgt((a), (b))))
  #define  vcmpgt4f(a, b)  ((vec4i)(vec_cmpgt((a), (b))))
  #define vcmpgt2lf __vcmpgt2lf

  /***** Greater Than Or Equal To *****/
  #define  vcmpge4i(a, b)  ((vec4i)(vec_cmpge((a), (b))))
  #define  vcmpge4f(a, b)  ((vec4i)(vec_cmpge((a), (b))))
  #define vcmpge2lf __vcmpge2lf

  /***** Less Than *****/
  #define  vcmplt4i(a, b)  ((vec4i)(vec_cmplt((a), (b))))
  #define  vcmplt4f(a, b)  ((vec4i)(vec_cmplt((a), (b))))
  #define vcmplt2lf __vcmplt2lf

  /***** Less Than Or Equal To *****/
  #define  vcmple4i(a, b)  ((vec4i)(vec_cmple((a), (b))))
  #define  vcmple4f(a, b)  ((vec4i)(vec_cmple((a), (b))))
  #define vcmple2lf __vcmple2lf


/*******************************************************************************
 *******************************************************************************
 ***** Mapping to Generic C Implementation
 *******************************************************************************
 *******************************************************************************/
#else

  /***** Data Types *****/
  typedef   __vec4i   vec4i;
  typedef   __vec4f   vec4f;
  typedef  __vec2lf  vec2lf;

  /***** Insert *****/
  #define  vinsert4i  __vinsert4i
  #define  vinsert4f  __vinsert4f
  #define vinsert2lf __vinsert2lf

  /***** Extract *****/
  #define  vextract4i  __vextract4i
  #define  vextract4f  __vextract4f
  #define vextract2lf __vextract2lf

  /***** Set *****/
  #define  vset4i  __vset4i
  #define  vset4f  __vset4f
  #define vset2lf __vset2lf

  /***** Constant Zero *****/
  #define  const_vzero4i  __const_vzero4i
  #define  const_vzero4f  __const_vzero4f
  #define const_vzero2lf __const_vzero2lf

  /***** Constant One *****/
  #define  const_vone4i  __const_vone4i
  #define  const_vone4f  __const_vone4f
  #define const_vone2lf __const_vone2lf

  /***** Constant Two *****/
  #define  const_vtwo4i  __const_vtwo4i
  #define  const_vtwo4f  __const_vtwo4f
  #define const_vtwo2lf __const_vtwo2lf

  /***** Constant Negative One *****/
  #define  const_vnegone4i  __const_vnegone4i
  #define  const_vnegone4f  __const_vnegone4f
  #define const_vnegone2lf __const_vnegone2lf

  /***** Rotate *****/
  #define  vroth4i  __vroth4i
  #define  vroth4f  __vroth4f
  #define vroth2lf __vroth2lf
  #define  vrotl4i  __vrotl4i
  #define  vrotl4f  __vrotl4f
  #define vrotl2lf __vrotl2lf
  
  /***** Addition *****/
  #define  vadd4i  __vadd4i
  #define  vadd4f  __vadd4f
  #define vadd2lf __vadd2lf

  /***** Subtraction *****/
  #define  vsub4i  __vsub4i
  #define  vsub4f  __vsub4f
  #define vsub2lf __vsub2lf

  /***** Multiplication *****/
  #define  vmul4f   __vmul4f
  #define vmul2lf  __vmul2lf

  /***** Division *****/
  #define  vdiv4f   __vdiv4f
  #define vdiv2lf  __vdiv2lf

  /***** Fused Multiply Add *****/
  #define  vmadd4f  __vmadd4f
  #define vmadd2lf __vmadd2lf

  /***** Reciprocal *****/
  #define  vrecip4f  __vrecip4f
  #define vrecip2lf __vrecip2lf

  /***** Square Root *****/
  #define  vsqrt4f  __vsqrt4f
  #define vsqrt2lf __vsqrt2lf

  /***** Reciprocal Square Root *****/
  #define  vrsqrt4f  __vrsqrt4f
  #define vrsqrt2lf __vrsqrt2lf

  /***** Not *****/
  #define  vnot4i  __vnot4i
  #define  vnot4f  __vnot4f
  #define vnot2lf __vnot2lf

  /***** Or *****/
  #define  vor4i  __vor4i
  #define  vor4f  __vor4f
  #define vor2lf __vor2lf

  /***** Nor *****/
  #define  vnor4i  __vnor4i
  #define  vnor4f  __vnor4f
  #define vnor2lf __vnor2lf

  /***** And *****/
  #define  vand4i  __vand4i
  #define  vand4f  __vand4f
  #define vand2lf __vand2lf

  /***** Nand *****/
  #define  vnand4i  __vnand4i
  #define  vnand4f  __vnand4f
  #define vnand2lf __vnand2lf

  /***** Xor *****/
  #define  vxor4i  __vxor4i
  #define  vxor4f  __vxor4f
  #define vxor2lf __vxor2lf

  /***** Nxor *****/
  #define  vnxor4i  __vnxor4i
  #define  vnxor4f  __vnxor4f
  #define vnxor2lf __vnxor2lf

  /***** Equal To *****/
  #define  vcmpeq4i  __vcmpeq4i
  #define  vcmpeq4f  __vcmpeq4f
  #define vcmpeq2lf __vcmpeq2lf

  /***** Greater Than *****/
  #define  vcmpgt4i  __vcmpgt4i
  #define  vcmpgt4f  __vcmpgt4f
  #define vcmpgt2lf __vcmpgt2lf

  /***** Greater Than Or Equal To *****/
  #define  vcmpge4i  __vcmpge4i
  #define  vcmpge4f  __vcmpge4f
  #define vcmpge2lf __vcmpge2lf

  /***** Less Than *****/
  #define  vcmplt4i  __vcmplt4i
  #define  vcmplt4f  __vcmplt4f
  #define vcmplt2lf __vcmplt2lf

  /***** Less Than Or Equal To *****/
  #define  vcmple4i  __vcmple4i
  #define  vcmple4f  __vcmple4f
  #define vcmple2lf __vcmple2lf


#endif


/*******************************************************************************
 *******************************************************************************
 ***** Shared Combinations
 *******************************************************************************
 *******************************************************************************/

/* NOTE: If any architecture specific implementation can do any of these
 *   operations faster, then move them up to the architecture specific areas and
 *   make individual definitions.  This area is just meant to declare commonly
 *   use combinations so that they don't have to be repeated many times over.
 */

/***** Spread (Duplicate functionality of 'Set' by another another name) *****/
#define  vspread4i(a)  ( vset4i(a))
#define  vspread4f(a)  ( vset4f(a))
#define vspread2lf(a)  (vset2lf(a))

/***** Add to Scalar *****/
#define   vadd4is(a, b)  ( vadd4i((a),  vset4i(b)))
#define   vadd4fs(a, b)  ( vadd4f((a),  vset4f(b)))
#define  vadd2lfs(a, b)  (vadd2lf((a), vset2lf(b)))

/***** Subtract a Scalar *****/
#define   vsub4is(a, b)  ( vsub4i((a),  vset4i(b)))
#define   vsub4fs(a, b)  ( vsub4f((a),  vset4f(b)))
#define  vsub2lfs(a, b)  (vsub2lf((a), vset2lf(b)))

/***** Multiply by Scalar *****/
#define   vmul4fs(a, b)  ( vmul4f((a),  vset4f(b)))
#define  vmul2lfs(a, b)  (vmul2lf((a), vset2lf(b)))

/***** Divide by Scalar *****/
#define  vdiv4fs(a, b)  ( vdiv4f((a),  vset4f(b)))
#define vdiv2lfs(a, b)  (vdiv2lf((a), vset2lf(b)))

/***** Fused Multiply(Vector) Add(Scalar) *****/
#define  vmadd4fs(a, b, c)  ( vmadd4f((a), (b),  vset4f(c)))
#define vmadd2lfs(a, b, c)  (vmadd2lf((a), (b), vset2lf(c)))

/***** Fused Multiply(Scalar) Add(Scalar) *****/
#define  vmadd4fss(a, b, c)  ( vmadd4f((a),  vset4f(b),  vset4f(c)))
#define vmadd2lfss(a, b, c)  (vmadd2lf((a), vset2lf(b), vset2lf(c)))





#endif //__SIMD_H__
