#define GDTBASE	0x800	/* addr. physique ou doit resider la gdt */
#define GDTSIZE	0xFF	/* nombre max. de descripteurs dans la table */

/* descripteur de segment */
struct gdtdesc {
	u16 lim0_15;	
	u16 base0_15;
	u8 base16_23;
	u8 acces;
	u8 lim16_19 : 4;
	u8 other : 4;
	u8 base24_31;
} __attribute__ ((packed));

/* registre GDTR */
struct gdtr {
	u16 limite ;
	u32 base ;
} __attribute__ ((packed));

struct gdtr kgdtr;

/* table de GDT */
struct gdtdesc kgdt[GDTSIZE] = {0, 0, 0, 0, 0, 0, 0};

/* pointeur sur un descripteur libre dans la GDT */
unsigned int kgdtptr = 1;
