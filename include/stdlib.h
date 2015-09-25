#ifndef	__STDLIB_H__
#define __STDLIB_H__

#include <stddef.h>
#include <compiler.h>

//Variable globales
char** environ;

DREAMOS_BEGIN_DECLS

//gestion de la memoire
void *calloc (size_t nmemb, size_t size);
void *malloc (size_t size);
void free (void *ptr);
void *realloc (void *ptr, size_t size);

//Fonctions supplemantaire
void qsort(void *array, size_t nelt, size_t eltsize, int (*cmp)(const void *,const void *) );
int abs(int j);
int tolower(int c);
char* getenv(const char *name);
void abort(void);

DREAMOS_END_DECLS


#endif /* stdlib.h  */
