#ifndef __STDDEF_H__
#define __STDDEF_H__

#ifndef NULL
# if !defined __cplusplus || defined __GNUC__ //Ces deux macros sont directement passé par le compilo
#  define NULL ((void *) 0)
# else
#  define NULL (0)
# endif
#endif

//TODO a supprimer une fois que l'on eclatera le kernel monolithique
#include "types.h"

//typedef unsigned int __size_t;
//typedef unsigned int size_t;
typedef long int time_t;
typedef long int __off_t;		/* Type of file sizes and offsets.  */

#endif 
