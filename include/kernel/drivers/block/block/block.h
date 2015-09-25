/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Pilotes bloc (IDE et disquettes)
*/

#ifndef __OWL_BLOCK_H
#define __OWL_BLOCK_H

#ifndef TESTDOS
#include <system.h> /* D�initions communes �tous les fichiers du noyau */
//#include <mm.h>     /* Gestionnaire de m�oire */
#include <klib.h>
#else
#include "testdos\system.h"
#include "testdos\test.h"
#endif

/*****************************************************************************/
/**                             Options                                     **/
/*****************************************************************************/

//#define FLOPPY_DRIVER            /* Pilote disquettes */
#define IDE_HD_DRIVER            /* Pilote disques durs IDE */
#define IDE_CD_DRIVER            /* Pilote CD-ROM IDE */

/*****************************************************************************/
/**                    Structures, types et d�initions                     **/
/*****************************************************************************/

/* Synchronisation automatique du cache au bout de CACHE_AUTO_SYNC pages */
#define CACHE_AUTO_SYNC  0x10000

#define CACHE_DEFAULT_SIZE 0x10

/* Model = 40 caract�es + \0 ; Serial = 20 caract�es + \0 */
#define MAX_MODEL_STR 41
#define MAX_SERIAL_STR 21

typedef u16 diskID_t;         /* ID d'un disque */
typedef u32 diskNbSect_t;     /* Nombre de secteurs */
typedef u32 lbaAddress_t;     /* Adresse logique (LBA) */

typedef enum{
#ifdef FLOPPY_DRIVER
  floppy,
#endif
#ifdef IDE_HD_DRIVER
  ideHd,
#endif
#ifdef IDE_CD_DRIVER
  ideCd,
#endif
  nothing
} diskType_t; /* Type de disque */

/* Priorit�dans le cache */
typedef enum{HIGHhigh=0, HIGHmedium, HIGHlow, MEDIUMhigh, MEDIUMmedium, MEDIUMlow, LOWhigh, LOWmedium, LOWlow} priorityLevel_t;

/*** Liste des disques d�ect� ***/
typedef struct __disks_list_entry
{
  diskID_t disk;                       /* ID du disque */
  struct __disks_list_entry *next;     /* Disque suivant */
} disksListEntry_t;

typedef struct __disks_list
{
  u32 nDisks;                        /* Nombre de disque */
  disksListEntry_t *disks;           /* Disques */
} disksList_t;

/** Fonctions support�s **/
typedef struct __disk_supported
{
  bool init;               /* Initialisation */
  bool read;               /* Lecture */
  bool write;              /* criture */
  bool getInf;             /* Obtenir des informations */
  bool getSize;            /* Obtenir la taille */
  bool lock;               /* Bloquer */
  bool unlock;             /* D�loquer */
  bool isChanged;          /* Savoir si le disque a chang�*/
} diskSupportedFeatures_t;

/** Informations sp�ifiques au type de disque **/

#ifdef FLOPPY_DRIVER
#define __FLOPPY_SPECIFIC
#include <drivers/block/floppy/floppy.h>
#undef __FLOPPY_SPECIFIC
#endif
#ifdef IDE_HD_DRIVER
#define __IDE_HD_SPECIFIC
#include <drivers/block/ide/ide.h>
#undef __IDE_HD_SPECIFIC
#endif
#ifdef IDE_CD_DRIVER
#define __IDE_CD_SPECIFIC
#include <drivers/block/ide/ide.h>
#undef __IDE_CD_SPECIFIC
#endif

typedef union __disk_type_specific
{
#ifdef FLOPPY_DRIVER
  floppySpecific_t floppy;
#endif
#ifdef IDE_HD_DRIVER
  ideHdSpecific_t ideHd;
#endif
#ifdef IDE_CD_DRIVER
  ideCdSpecific_t ideCd;
#endif
  u8 nothing;
} diskTypeSpecific_t;

/** Informations sur un disque ***/
typedef struct __disk
{
  diskID_t id;                              /* ID */
  diskNbSect_t nSects;                      /* Nombre de secteurs */
  diskType_t type;                          /* Type */
  u8 model[MAX_MODEL_STR];                  /* Mod�e */
  u8 serial[MAX_SERIAL_STR];                /* Num�o de s�ie */
  diskTypeSpecific_t spec;                  /* Informations sp�ifiques */
  diskSupportedFeatures_t features;         /* Fonctions support�s */
} disk_t;

/** Entr� du cache **/
typedef struct __cache
{
  u32 id;                       /* ID de l'entr� */
  u8 isUsed;                    /* Cette entr� est-elle utilis� ? */
  diskID_t dId;                 /* ID du disque */
  lbaAddress_t log;             /* Adresse logique du secteur */
  priorityLevel_t priority;     /* Niveau de priorit�*/
  u16 access;                   /* Nombre d'acc� */
  u8 sector[512];               /* Secteur */
} blockDriverCache_t;

/* Erreurs */

#define BLOCK_ERR_OK       0       /* Pas d'erreur */
#define BLOCK_ERR_DATA     1       /* Donn�s incorrectes */
#define BLOCK_ERR_UNIMPLEMENTED 2  /* Commande interrompue  ou non impl�ent� */
#define BLOCK_ERR_LBA      3       /* Adresse LBA incorrecte, pas de secteur */
#define BLOCK_ERR_DISK     4       /* Le disque n'existe pas ou n'est pas pr� */
#define BLOCK_ERR_SIZE     5       /* Nombre de secteurs trop grand ou trop petit */
#define BLOCK_ERR_WRITE    6       /* Protection en �riture */
#define BLOCK_ERR_UNINITIALIZED 7  /* Pilote non initialis�*/
#define BLOCK_ERR_MEMORY   8       /* M�oire insuffisante */
#define BLOCK_ERR_UNKNOWN_DRIVER 9 /* Pilote inconnu */
#define BLOCK_ERR_NO_MEDIA 10      /* Lecteur pr�ent mais ne contient pas de disque (disques amovibles) */
#define BLOCK_ERR_UNKNOWN_ERROR 11 /* Erreur ind�inie */

/*****************************************************************************/
/**                           Declarations                                  **/
/*****************************************************************************/

/* Messages d'erreurs */
extern u8 *blockErrorsStr[];

/* Initialisation */
extern s8 blockIsInitialized;

/* Cache */
extern blockDriverCache_t *blockSache;         /* Cache */
extern u32 blockCacheSize;                     /* Taille du cache */
extern u32 blockCacheCurrentId;                /* Num�o d'entr� courant */

/* Liste des disques */
extern disksList_t blockDisksList;

/* Initialiser le driver */
u8                           /* Retour : code d'erreur */
    blockInitDriver(
      u32                    /* Nombre maximum de secteurs dans le cache */
    );

/* D�harger le pilote */
u8                           /* Retour : code d'erreur */
    blockCleanupDriver(
      void
    );

/* Obtenir une place pour une nouvelle entr� dans le cache */
u8                           /* retour : code d'erreur */
    blockNewCacheEntry(
      blockDriverCache_t **  /* Pointeur vers l'emplacement selectionn�*/
    );

/* Synchroniser le cache et les disques */
u8                           /* Retour : code d'erreur */
    blockSync(
      void
    );

/* Obtenir des infos sur un disque */
u8                           /* Retour : code d'erreur */
    blockGetInf(
      diskID_t,              /* ID du disque */
      disk_t *               /* Structure pour le r�ultat */
    );

/* Initialiser un disque */
u8                           /* Retour : code d'erreur */
    blockInit(
      diskID_t               /* ID du disque */
    );

/* Lire depuis un disque, niveau LOWlow */
u8                           /* Retour : code d'erreur */
    blockRead(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1er secteur �lire */
      u8 *                   /* Tampon m�oire */
    );

/* Lire depuis un disque (sp�ifier le niveau de priorit� */
u8                           /* Retour : code d'erreur */
    blockReadWithPriority(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1er secteur �lire */
      u8 *,                  /* Tampon m�oire */
      priorityLevel_t        /* Niveau de priorit�des secteurs lus */
    );

/* crire sur un disque, niveau LOWlow */
u8                           /* Retour : code d'erreur */
    blockWrite(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1er secteur ��rire */
      u8 *                   /* Tampon m�oire */
    );

/* crire sur un disque sans passer par le cache */
u8                           /* Retour : code d'erreur */
    blockWriteDirectly(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1er secteur ��rire */
      u8 *                   /* Tampon m�oire */
    );

/* crire sur un disque (sp�ifier le niveau de priorit� */
u8                           /* Retour : code d'erreur */
    blockWriteWithPriority(
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1er secteur ��rire */
      u8 *,                  /* Tampon m�oire */
      priorityLevel_t        /* Niveau de priorit�des secteurs ��rire */
    );

/* V�ification de l'existance d'un disque */
u8                           /* 0 ou 1 */
    blockDiskExists(
      diskID_t               /* Disque */
    );

/* Obtenir la taille en blocs de 512 octets d'un disque */
u8                           /* Retour : code d'erreur */
    blockGetSize(
      diskID_t,              /* Disque */
      u32 *                  /* Emplacement pour le r�ultat */
    );

/* Bloquer le m�ia */
u8                           /* Retour : code d'erreur */
    blockLock(
      diskID_t               /* Disque */
);

/* D�loquer le m�ia */
u8                           /* Retour : code d'erreur */
    blockUnlock(
      diskID_t               /* Disque */
);

/* le mdia a-t-il chang ? */
s8                           /* retour : -(code d'erreur), 0 (non), 1 (oui) */
    blockIsChanged(
      diskID_t               /* Disque */
    );

#ifdef FLOPPY_DRIVER
#include <drivers/block/floppy/floppy.h>
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
#include <drivers/block/ide/ide.h>
#endif

#endif
