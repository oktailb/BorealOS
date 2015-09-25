#ifndef __SYSTEM_H__
#define __SYSTEM_H__

//TODO ne devrait pas etre inclu la mais comme historiquement system.h existe 
//depuis bien plus longtemps que types.h il faut tout modifier et j'ai la flemme
#include "types.h"

//macro generic
#ifndef NULL
#define NULL 0x0
#endif

//definitions de fonctions bas niveau
void cli(void);
void sti(void);
void outb(u16 port,u8 value);
void outbp(u16 port,u8 value);
u8 inb(u16 port);
u8 inbp(u16 port);
void outl(u16 port,u32 value);
void outlp(u16 port,u32 value);
u32 inl(u16 port);
u16 inw(u16 port);
void outwp(u16 port,u16 value);

typedef struct __indirect_t {
    unsigned long address;
    unsigned short segment;
}__attribute__((packed)) indirect;


#endif
