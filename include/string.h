#ifndef __STRING_H__
#define __STRING_H__

#include <stddef.h>
#include <compiler.h>

DREAMOS_BEGIN_DECLS

extern size_t strlen(const char *__s);
extern char *strcpy(char *__dest, const char *__src);
extern char *strncpy(char *__dest, const char *__src, size_t __n);
extern char *strdup(const char *__s);
extern char *strcat(char *__dest, const char *__src);
extern char *strncat(char *__dest, const char *__src, size_t __n);
extern int strcmp(const char *__a, const char *__b);
extern int strncmp(const char *__a, const char *__b, size_t __n);

extern char *strchr(const char *__s, int __c);
extern char *strrchr(const char *__s, int __c);
extern char *strstr(const char *__haystack, const char *__needle);
extern char *strtok(char *__s, const char *__delim);
extern char *strtok_r(char *__s, const char *__delim, char **__last);
extern char *strpbrk(const char *__s1, const char *__s2);
extern size_t strspn(const char *__s1, const char *__s2);
extern size_t strcspn(const char *__s1, const char *__s2);

extern void *memmove(void *__to, const void *__from, size_t __n);
extern void *memchr(const void *b, int c, size_t len);
extern int memcmp(const void *s1v, const void *s2v, size_t size);
extern void *memset(void *tov, int c, size_t len);
extern void *memcpy(void *dest, const void *src, size_t n);
extern void *memccpy(void *t, const void *f, int c, size_t n);

extern char *strerror(int __errno);

DREAMOS_END_DECLS

#endif 
