#include "klib.h"
#include "system.h"
#include "consol.h"

/* ------------------- OWL  -  D O C -------------------------------
** Nom        : kFault
** Entree(s)  : AUCUNE
** Sortie(s)  : AUCUNE
** Val de ret : AUCUNE
** Sujet      : arrete le system apres une erreur irecuperable
** Dependance : 
** Auteur     : Guillaume DORCHIES
** Version    : 0.1
** TODO       : il faudrait emmettre en son au cas ou on ne peut plus ecrire sur l'écran
------------------------------------------------------------------------*/
inline void kFault() {
  sti(); //pour etre sur que tout s'arrete en l'état
  kPrint("ERREUR FATAL GAME OVER\n");
  while(1);
}


/* ------------------- OWL  -  D O C -------------------------------
** Nom        : k_memset
** Entree(s)  : addresse du bloc
**              byte d'initialisation
**              taille du bloc
** Sortie(s)  : AUCUNE
** Val de ret : AUCUNE
** Sujet      : initialise un bloc memoire
** Dependance : 
** Auteur     : Guillaume DORCHIES
** Version    : 0.1
------------------------------------------------------------------------*/
void *kMemset(void *tov, int c, int len)
{
	register char *to = tov;

	while (len-- > 0)
		*to++ = c;

	return tov;
}


/* ------------------- OWL  -  D O C -------------------------------
** Nom        : k_memcpy
** Entree(s)  : addresse de destination
**              addresse de la source
**              taille du bloc à copier
** Sortie(s)  : AUCUNE
** Val de ret : AUCUNE
** Sujet      : copie un bloc memoire d'une certaine taille d'une addresse à une autre
** Dependance : 
** Auteur     : Guillaume DORCHIES
** Version    : 0.1
------------------------------------------------------------------------*/
void kMemcpy(void *dest,void *src, int n)
{
	char *s, *d;
	int i;

	s = (char*) src;
	d = (char*) dest;

	for(i=0;i<n;i++){
		*(d+i) = *(s+i);	
	}

	return;
}

/* ------------------- OWL  -  D O C -------------------------------
** Nom        : reboot
** Entree(s)  : AUCUNE
** Sortie(s)  : AUCUNE
** Val de ret : Rien
** Sujet      : Reboot la machine sans recourir au bios
** Dependance : AUCUNE
** Auteur     : Steve Hostettler d'apres une fonction de ...
** Creation   : 06.01.2001
** Version    : 0.2
------------------------------------------------------------------------*/
void kReboot()
{
        int c;
	cli();
        for( c=0; c< 0xFFFF; c++)
                if( inb(0x64) == 0x02)
                        break;
        outb(0x64,0xFE);
	sti();
        while(1);

        return;
}

/* ------------------- OWL  -  D O C -------------------------------
** Nom        : kmalloc
** Entree(s)  : TAILLE DE LA ZONE MEMOIRE A RESERVE(u32)
** Sortie(s)  : AUCUNE
** Val de ret : UN POINTEUR SUR LA ZONE RESERVE(void *)AUCUNE
** Sujet      : Reserve une quantite de memoire sur le tas.
**              Et renvoie le pointeur sur la zone reserve.
** Dependance : AUCUNE
** Auteur     : Steve Hostettler
** Creation   : 22.08.2000
** Version    : 0.2
------------------------------------------------------------------------*/
void* kMalloc(u32 size)  {
  static void *memptr=(void *)0x600000;
  void *ptr=memptr;
  memptr=memptr+size;
  return ptr;
}

/* ------------------- OWL  -  D O C -------------------------------
** Nom        : kfree
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
**              
** Dependance : 
** Auteur     : 
** Creation   : 
** Version    : 
------------------------------------------------------------------------*/
void kFree()  {

}


/* ------------------- OWL  -  D O C -------------------------------
** Nom        : k_sleep
** Entree(s)  : time (temps d'attente en milliseconde)
** Sortie(s)  : AUCUNE
** Val de ret : AUCUNE
** Sujet      : Fait une pause 
** Dependance : AUCUNE
** Auteur     : Dorchies Guillaume
** Creation   : 8/8/2001
** Version    : 0.1
** TODO       : j'espere que la boucle interne fait bien un delay d'une miliseconde
                en fait c'est totalement non déterministe car il y a trop de paramétre
		il faudrat trouver autre chose :-)) mais pour l'instant cela fait une pause d'une durée inconnue 
		prendre une valeur < à 30 pour ne pas avoir besoin d'aller chercher un café
------------------------------------------------------------------------*/
void kSleep(u32 time)
{
#define SPEED 0x8
// 0x30 is medium
// 0x8 is fast

	int i,j;
	volatile int k;
	for(i=0; i<(time*0x1000); i++)
		for(j=0; j<SPEED; j++)
			k=i*j;
#undef SPEED
}
