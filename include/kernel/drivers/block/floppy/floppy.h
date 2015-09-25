/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Pilote disquette
*/

/*
  Ce fichier est d'abord inclus par block.h avec __FLOPPY_SPECIFIC pour déclarer la struct de données spécifiques au type de disque puis une seconde fois à la fin du fichier pour déclarer le reste. Cette structure est rendue obligatoire du fait de nombreuses interactions entre les deux fichiers
*/ 

/*****************************************************************************/
/**                Donnees specifiques au type de disque                    **/
/*****************************************************************************/

#ifdef __FLOPPY_SPECIFIC

typedef enum{nec765, extAdapter} adapterType;

typedef struct __floppy_specific
{
  adapterType type;
} floppySpecific_t;

#elif !defined(__OWL_FLOPPY_H)
#define __OWL_FLOPPY_H

#ifndef TESTDOS
#include <klib.h>
//#include <dma.h>
#endif
#include "block.h"

/*****************************************************************************/
/**                           Definitions                                   **/
/*****************************************************************************/

#define DG144_HEADS       2     /* tete par lecteur (1.44M) */
#define DG144_TRACKS     80     /* nbre de pistes (1.44M) */
#define DG144_SPT        18     /* secteurs par pistes (1.44M) */
#define DG144_GAP3FMT  0x54     /* gap3 formate (1.44M) */
#define DG144_GAP3RW   0x1b     /* gap3 lecture ecriture (1.44M) */

#define DG168_HEADS       2     
#define DG168_TRACKS     80     
#define DG168_SPT        21     
#define DG168_GAP3FMT  0x0c     
#define DG168_GAP3RW   0x1c     

/* IO ports */
#define FDC_DOR  (0x3f2)   
#define FDC_MSR  (0x3f4)   
#define FDC_DRS  (0x3f4)   
#define FDC_DATA (0x3f5)   
#define FDC_DIR  (0x3f7)   
#define FDC_CCR  (0x3f7)   

#define CMD_SPECIFY (0x03)  /* timing du lecteur*/
#define CMD_WRITE   (0xc5)  /* ecris une donnee */
#define CMD_READ    (0xe6)  /* lis une donnee */
#define CMD_RECAL   (0x07)  /* recalibrer */
#define CMD_SENSEI  (0x08)  /* statut interruption */
#define CMD_FORMAT  (0x4d)  /* formater une piste */
#define CMD_SEEK    (0x0f)  /* chercher une piste */
#define CMD_VERSION (0x10)  /* version fdc */

/* Timeout provisoire */
#define FLOPPY_TIMEOUT 10000000

typedef struct __floppy_drv_geom
{
  u8 heads;
  u8 tracks;
  u8 spt;
} floppyDrvGeom_t;

/*****************************************************************************/
/**                           Declarations                                  **/
/*****************************************************************************/

extern disksList_t floppyDisksList;

/* Initialiser le driver */
u8                           /* Retour : code d'erreur */
    floppyInitDriver(
      void
    );

/* Décharger le pilote */
u8                           /* Retour : code d'erreur */
    floppyCleanupDriver(
      void
    );

/* Obtenir des infos sur un disque */
u8                           /* Retour : code d'erreur */
    floppyGetInf(
      diskID_t,              /* ID du disque */
      disk_t *               /* Structure pour le resultat */
    );

/* Initialiser un disque */
u8                           /* Retour : code d'erreur */
    floppyInit(
      diskID_t               /* ID du disque */
    );

/* Lire un disque */
u8                           /* Retour : code d'erreur */
    floppyRead(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1e secteur a lire */
      u8 *                   /* Tampon memoire */
    );

/* Ecrire un disque */
u8                           /* Retour : code d'erreur */
    floppyWrite(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1e secteur a ecrire */
      u8 *                   /* Tampon memoire */
    );

/* Obtenir la taille en blocs de 512 octets d'un disque */
u8                           /* Retour : code d'erreur */
    floppyGetSize(
      diskID_t,              /* Disque */
      u32 *                  /* Emplacement pour le résultat */
    );

/* Bloquer le média */
u8                           /* Retour : code d'erreur */
    floppyLock(
      diskID_t               /* Disque */
);

/* Débloquer le média */
u8                           /* Retour : code d'erreur */
    floppyUnlock(
      diskID_t               /* Disque */
);

/* le m‚dia a-t-il chang‚ ? */
s8                           /* retour : -(code d'erreur), 0 (non), 1 (oui) */
    floppyIsChanged(
      diskID_t               /* Disque */
    );

/* Fonctions du pilote d'origine (Christophe Guelff) */

void floppyLowReset(void);
void floppyLowMotorOn(void);
void floppyLowMotorOff(void);
void floppyLowRecalibrate(void);
bool floppyLowSeek(u8 track);
bool floppyLowLogDisk(floppyDrvGeom_t *g);
bool floppyLowReadBlock(u32 block,u8 *blockBuff);
bool floppyLowWriteBlock(u32 block,u8 *blockBuff);
bool floppyLowRW(u32 block, u8 *blockBuff, bool read);
void floppyLowSendByte(u8);
s8 floppyLowGetByte(void);
bool floppyLowWaitFDC(bool);
void floppyLowIRQ6(void);
void floppyLowConvertBlockToHTS(u32 block, u8 *head, u8 *track, u8 *sector);

#endif

