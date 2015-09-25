#ifndef __KLIB_H__
#define __KLIB_H__


#include "system.h"

void kMemcpy(void* dest,void* src,int length);
void *kMemset(void *tov, int c, int len);
void kReboot(void);
void* kMalloc(u32 size);
void kFree();
void kSleep(u32 time);
void kFault();
#endif
