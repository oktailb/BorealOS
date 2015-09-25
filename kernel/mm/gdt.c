#include "../include/types.h"
#include "../include/gdt.h"

void memcopy(void*, void*, int);

/*
 * 'init_desc' initialise un descripteur de segment situe en gdt ou en ldt.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 */
void init_gdt_desc(u32 base, u32 limite, u8 acces, u8 other, struct gdtdesc* desc) {
	desc->lim0_15 = (limite & 0xffff);
	desc->base0_15 = (base & 0xffff);
	desc->base16_23 = (base & 0xff0000)>>16;
	desc->acces = acces;
	desc->lim16_19 = (limite & 0xf0000)>>16;
	desc->other = (other & 0xf);
	desc->base24_31 = (base & 0xff000000)>>24;
	return;
}

/*
 * 'init_code_desc' initialise un descripteur de segment de code.
 * 'init_data_desc' initialise un descripteur de segment de data.
 * 'desc' est l'adresse lineaire du descripteur a initialiser.
 */
void init_code_desc(u32 base, u32 limite, struct gdtdesc* desc) {
	init_gdt_desc(base, limite, 0x9B, 0x0D, desc);
}

void init_data_desc(u32 base, u32 limite, struct gdtdesc* desc) {
	init_gdt_desc(base, limite, 0x93, 0x0D, desc);
}

/*
 * 'add_gdt_desc' ajoute dans la GDT le descripteur passe en argument.
 * 'kgdtptr', definie dans 'gdt.h' est une variable globale correspondant
 * a un index dans la GDT. Elle pointe sur un descripteur vacant dans 
 * la table. Attention, aucun controle n'est effectue pour empeche
 * les debordements de la GDT.
 */
void add_gdt_desc(struct gdtdesc desc) {
	kgdt[kgdtptr] = desc;
	kgdtptr++;
}

/*
 * Cette fonction initialise la GDT apres que le kernel soit charge 
 * en memoire. Une GDT est deja operationnelle, mais c'est celle qui
 * a ete initialisee par le secteur de boot et qui ne correspond
 * pas forcement a celle que l'on souhaite pour bosokernel.
 */
void init_gdt(void) {
	struct gdtdesc code, data, stack;

	/* initialisation des descripteurs de segment */
	init_code_desc(0x0, 0xFFFFF, &code);
	init_data_desc(0x0, 0xFFFFF, &data);
	init_gdt_desc(0, 0x10, 0x97, 0x0D, &stack);
	add_gdt_desc(code);
	add_gdt_desc(data);
	add_gdt_desc(stack);

	/* initialisation de la structure pour GDTR */
	kgdtr.limite = GDTSIZE*8;
	kgdtr.base = GDTBASE;

	/* recopie de la GDT a son adresse */	
	memcopy(kgdt, kgdtr.base, kgdtr.limite);
	
	/* chargement du registre GDTR */
	asm("lgdtl (kgdtr)");

	/* initialisation des segments */
	asm("	movw $0x10,%ax	\n \
		movw %ax, %ds	\n \
		movw %ax, %es	\n \
		movw %ax, %fs	\n \
		movw %ax, %gs	\n \
		movw $0x18,%ax	\n \
		movw %ax, %ss	\n \
		movl $0x1FFFF,%esp	\n \
		nop	\n \
		nop	\n \
		ljmp $0x08,$next	\n \
		next:	\n");
}
