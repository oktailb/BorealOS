%define CS_ACCES        10011011b
%define DS_ACCES        10010011b

[BITS 16]
[ORG 0x0]

	jmp	start

;---------------------------------------------------------
; Synopsis:	Affiche une chaine de caracteres se terminant par 0x0
; Entree:	ds:si -> pointe sur la chaine a afficher
;---------------------------------------------------------
afficher:
	push	ax
	push	bx
.debut:
	lodsb		;; ds:si -> al
	cmp	al,0	;; fin chaine ?
	jz	.fin
	mov	ah,0x0E	;; appel au service 0x0e, int 0x10 du bios
	mov	bx,0x07	;; bx -> attribut, al -> caractere ascii
	int	0x10
        jmp	.debut

.fin:
	pop	bx
	pop	ax
	ret

;--------------------------------------------------------------------
; CalcAdr
; Entree:
;	ax -> segment
;	bx -> offset
; Sortie:
;	ecx -> adresse lineaire sur 32 bits
; Modifie: partie haute de ebx
;--------------------------------------------------------------------
calcadr:
	xor	ecx,ecx
	mov	cx,ax
	shl	ecx,4
	and	ebx,0x0000FFFF
	add	ecx,ebx
	ret
;--------------------------------------------------------------------
	
;--------------------------------------------------------------------
; descInit
;--------------------------------------------------------------------
%macro descInit	5	;; base(32),limite(20/32),acces(8),flags(4/8),adresse(32)
	push	eax
; base :
	mov	eax,%1
	mov	word [%5+2],ax
	shr	eax,16
	mov	byte [%5+4],al
	shr	eax,8
	mov	byte [%5+7],al
; limite :
	mov	eax,%2
	and	eax,0x000FFFFF
	mov	word [%5],ax	; ecrit (0..15)
	shr	eax,16			; place (16..19) sur le nibble inferieur
	mov	byte [%5+6],0	; initialise flags+lim(16..19) a 0
	or	[%5+6],al		; ecrit (16..19)
; flags :
	mov	al,%4
	and	al,0x0F
	shl	al,4
	or	[%5+6],al
; acces :
	mov	byte [%5+5],%3
	pop	eax
%endmacro
;--------------------------------------------------------------------


%define LDT_FLAG	0x2		; ldt
%define TASK_FLAG	0x5		; task gate
%define TSS_FLAG	0x9		; TSS
%define CALL_FLAG	0xC		; call gate
%define INT_FLAG	0xE		; interrupt gate
%define TRAP_FLAG	0xF		; trap gate

%define PRESENT_F	0x80	; present flag
%define	DPL0		0x00
%define	DPL1		0x20
%define	DPL2		0x40
%define	DPL3		0x60

;--------------------------------------------------------------------
; intdesc
;--------------------------------------------------------------------
%macro gateInit	5	; selecteur(16),offset(32),type (8),wc(5/8),adresse(32)
	push	eax
;; selecteur :
	mov	word [%5+2],%1
;; offset :
	mov	eax,%2
	mov	word [%5],ax
	shr	eax,16
	mov	word [%5+6],ax
;; type :
	mov	byte [%5+5],%3
;; wc :
	mov	byte [%5+4],%4
	and	byte [%5+4],00011111b
	pop	eax
%endmacro
;--------------------------------------------------------------------

error:
	mov	si,msgError
	call	afficher
fail:	
	jmp	fail

start:
	mov	[bootdrv],dl	;; recuparation de l'unite de boot

	;; initialisation des segments en [kbase]
	mov	ax,0x07C0
	mov	ds,ax
	mov	es,ax
	mov	ax,0x8000	;; stack en 0xFFFF
	mov	ss,ax
	mov	sp, 0xf000

	;; affiche un msg
	mov	si,msgDebut
	call	afficher

	cmp	dl,2	 
	jb	_floppy		;; 	Si [bootdrv]=0 ou 1, il s'agit d'une disquette
	jmp	_dd		;; 	Sinon -> dd
 	 
_floppy:	 
	mov	bx,[kbase]	;; 	Offset
	mov	ah,2		;; 	Lecture
	mov	al,[ksize]	;; 	1 secteur
	mov	dh,[head]	;; 	Tete
	mov	cx,[tr_sc]	;; 	Cylindre et secteur
	int	13h	 
	jmp	_init	 
 	 
_dd:	;; 	Lancement depuis un disque dur
	mov	al, [part]	;; 	Numero de la partition
	cmp	al,1	 
	jb	error	 
	cmp	al,4	 
	ja	error	 
 	 
;; on se deplace a l'entree correspondante a la partition	 
	mov	di,[table]	 
	mov	ch,0	 
	mov	cl,[part]	;; 	numero de la partition
nextpart:	 
	add	di,10h		;; 	Une entree de la table des partitions contient 10h (16) octets
	loop	nextpart	 
 	 
;; es:[di] pointe maintenant vers l'entree correspondante de la table
	inc	di	 
	mov	dh, [di]	;; 	Tete
	inc	di	 
	mov	cx, [di]	;; 	Cylindre et secteurs
	mov	ax,[kbase]	 
	mov	es,ax		;; 	Segment
	mov	bx,[koffset]	;; 	Offset
	mov	ah,2		;; 	Lecture
	mov	al,[ksize]	;; 	Lire ksize secteurs
	int	13h		;; 	->Lire
	jc	error		;; 	Si erreur

_init:	
; initialisation de la GDT
	; descInit	base(32),	limite(20/32),	acces(8),	flags(4/8),	adresse(16)
	descInit	0,		0xFFFFF,	CS_ACCES,	1101b,		gdt_cs
	descInit	0,		0xFFFFF,	DS_ACCES,	1101b,		gdt_ds

; initialisation du pointeur sur la GDT
	mov	ax,gdtend	; calcule la limite de GDT
	mov	bx,gdt
	sub	ax,bx
	mov	word [gdtptr],ax

	xor	eax,eax		; calcule l'adresse lineaire de GDT
	mov	ax,ds
	mov	bx,gdt
	call	calcadr
	mov	dword [gdtptr+2],ecx

	mov	si,msgSuccess
	call	afficher

; passage en modep
	cli
	lgdt	[gdtptr]	; charge la gdt
	mov	eax,cr0
	or	ax,1
	mov	cr0,eax		; PE mis a 1 (CR0)

	jmp	next
next:
	mov	ax,0x10		; segment de donnee
	mov	ds,ax
	mov	fs,ax
	mov	gs,ax
	mov	es,ax
	mov	ss,ax
	mov	esp,0x9F000
	jmp	dword 0x8:0x1000
end:
	jmp	end

msgDebut	db	"Boot ",0
msgError	db	"Fail",0
msgSuccess	db	"success",0
;--------------------------------------------------------------------
;; NOP jusqu'a 446
times 446-($-$$) db 144		; reste du code

gdt:
gdt_null:
	dw 0,0,0,0
gdt_cs:
	dw 0,0,0,0
gdt_ds:
	dw 0,0,0,0
gdtend:

gdtptr:
	dw	0x1000		; limite
	dd	0x1000		; base

kbase:		dw	0x1000
koffset:	dw	0	
ksize:		db	30
part:		db	1
head:		db	0
bootdrv:	db	0
tr_sc:		dw	0

mbr:	 
table:				;; 	Voici la table des partitions
part1:
	part1_boot_flag:	db 0	;; 	0 = inactive, 80h = active
	part1_begin_head:	db 0	;; 	T-bête de début-A
	part1_begin_sector:	db 0	;;	Secteur de d-bébut (*)-A
	part1_begin_cyl:	db 0	;; 	Cylindre de d-bébut (*)-A
	part1_type:		db 0	;; 	Type : 83h = Linux, bh = Win95/FAT32, ...	
	part1_end_head:		db 0	;; 	T-bête de fin-A
	part1_end_sector:	db 0	;; 	Secteur de fin (*)
	part1_end_cyl:		db 0	;; 	Cylindre de fin (*)
	part1_start_sector:	dd 0	;; 	Nombre de secteurs avant la partition (donc num-béro du 1e secteur) (**)-A
	part1_sector_count:	dd 0	;; 	Nombre de secteurs dans la partition (**)
part2:	 
	part2_boot_flag:	db 0	;; 	0 = inactive, 80h = active
	part2_begin_head:	db 0	;; 	T-bête de début-A
	part2_begin_sector:	db 0	;;	Secteur de d-bébut (*)-A
	part2_begin_cyl:	db 0	;; 	Cylindre de d-bébut (*)-A
	part2_type:		db 0	;; 	Type : 83h = Linux, bh = Win95/FAT32, ...	
	part2_end_head:		db 0	;; 	T-bête de fin-A
	part2_end_sector:	db 0	;; 	Secteur de fin (*)
	part2_end_cyl:		db 0	;; 	Cylindre de fin (*)
	part2_start_sector:	dd 0	;; 	Nombre de secteurs avant la partition (donc num-béro du 1e secteur) (**)-A
	part2_sector_count:	dd 0	;; 	Nombre de secteurs dans la partition (**)
part3:	 
	part3_boot_flag:	db 0	;; 	0 = inactive, 80h = active
	part3_begin_head:	db 0	;; 	T-bête de début-A
	part3_begin_sector:	db 0	;;	Secteur de d-bébut (*)-A
	part3_begin_cyl:	db 0	;; 	Cylindre de d-bébut (*)-A
	part3_type:		db 0	;; 	Type : 83h = Linux, bh = Win95/FAT32, ...	
	part3_end_head:		db 0	;; 	T-bête de fin-A
	part3_end_sector:	db 0	;; 	Secteur de fin (*)
	part3_end_cyl:		db 0	;; 	Cylindre de fin (*)
	part3_start_sector:	dd 0	;; 	Nombre de secteurs avant la partition (donc num-béro du 1e secteur) (**)-A
	part3_sector_count:	dd 0	;; 	Nombre de secteurs dans la partition (**)
part4:	 
	part4_boot_flag:	db 0	;; 	0 = inactive, 80h = active
	part4_begin_head:	db 0	;; 	T-bête de début-A
	part4_begin_sector:	db 0	;;	Secteur de d-bébut (*)-A
	part4_begin_cyl:	db 0	;; 	Cylindre de d-bébut (*)-A
	 part4_type:		db 0	;; 	Type : 83h = Linux, bh = Win95/FAT32, ...	
	part4_end_head:		db 0	;; 	T-bête de fin-A
	part4_end_sector:	db 0	;; 	Secteur de fin (*)
	part4_end_cyl:		db 0	;; 	Cylindre de fin (*)
	part4_start_sector:	dd 0	;; 	Nombre de secteurs avant la partition (donc num-béro du 1e secteur) (**)-A
	part4_sector_count:	dd 0	;; 	Nombre de secteurs dans la partition (**)
signature:	
	dw 0xAA55
