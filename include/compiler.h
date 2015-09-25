#ifndef __COMPILER_H__
#define __COMPILER_H__

/*
 * All function prototypes should be surrounded by these macros,
 * so that a C++ compiler will know that they're C functions.
 */
#ifdef __cplusplus
#define DREAMOS_BEGIN_DECLS extern "C" {
#define DREAMOS_END_DECLS }
#else
#define DREAMOS_BEGIN_DECLS
#define DREAMOS_END_DECLS
#endif


#endif 
