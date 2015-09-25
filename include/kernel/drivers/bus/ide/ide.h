/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Pilote IDE
*/

/*
  Ce fichier est d'abord inclus par block.h avec __IDE_HD_SPECIFIC ou __IDE_CD_SPECIFIC pour declarer la struct de donnees specifiques au type de disque puis une seconde fois a la fin du fichier pour declarer le reste. Cette structure est rendue obligatoire du fait de nombreuses interactions entre les deux fichiers
*/ 

/*****************************************************************************/
/**                Donnees specifiques au type de disque                    **/
/*****************************************************************************/

#if (defined(__IDE_HD_SPECIFIC) || defined(__IDE_CD_SPECIFIC)) && !defined(IDE_PHYSICALDISKID)

#define IDE_PHYSICALDISKID
typedef unsigned char idePhysicalDiskID_t;

#endif

#ifdef __IDE_HD_SPECIFIC

typedef struct __ide_hd_specific
{
  u8 lba;
  lbaAddress_t start;
  idePhysicalDiskID_t pId;
  u16 heads;
  u16 cyls;
  u16 sectPerCyl;
  u8 partBIOS;
} ideHdSpecific_t;

#elif defined(__IDE_CD_SPECIFIC)

typedef struct __ide_cd_specific
{
  idePhysicalDiskID_t pId;
} ideCdSpecific_t;

#elif !defined(__OWL_IDE_H)
#define __OWL_IDE_H

#include "block/block.h"

#ifdef IDE_CD_DRIVER
#include "packetcd/packetcd.h"
#endif

/*****************************************************************************/
/**                           Definitions                                   **/
/*****************************************************************************/

#define IDE_MAX_ADAPTERS 4

#define IDE_DATA        0
#define IDE_ERR         1
#define IDE_FEATURES    1
#define IDE_COUNT       2
#define IDE_SECT        3
#define IDE_CYLLOW      4
#define IDE_CYLHIGH     5
#define IDE_HEADDSK     6
#define IDE_STAT        7
#define IDE_COMMAND     7
#define IDE_STAT2       0x206
#define IDE_CTRL        0x206

#define IDE_COMM_RECAL         0x10
#define IDE_COMM_READ          0x20
#define IDE_COMM_WRITE         0x30
#define IDE_COMM_GETINF        0xec
#define IDE_COMM_PACKET        0xa0
#define IDE_COMM_GETINF_PACKET 0xa1

#define IDE_MASTER      0
#define IDE_SLAVE       1

#define ATA_PROTOCOL    0
#define ATAPI_PROTOCOL  1

#define IDE_TIMEOUT     10000000 /* Nombre d'essai avant de conclure que le disque n'est pas prˆt */

/* Attendre 400ns */
#define L400NS(x) __asm__("inb %%dx, %%al\ninb %%dx, %%al\ninb %%dx, %%al\ninb %%dx, %%al\n" : : "d" (x))

#define PACKET_TRY_AGAIN 10 /* Nombre d'essai avant de conclure … l'echec d'un paquet */

#define DEVICE_ATAPI_CDROM     0x5

typedef enum {chs, lba} ideAddressType_t;
typedef enum {itt_no, itt_read, itt_write} ideTransfType_t;

typedef struct __ide_param
{
  u8 nSects;                  /* Nombre de secteurs */
  ideAddressType_t adrType;
  u8 sector;                  /* Secteur ou LBA 0-7 */
  u8 cylLow;                  /* Cylindre (octet inferieur) ou LBA 8-15 */
  u8 cylHigh;                 /* Cylindre (octet superieur) ou LBA 16-23 */
  u8 head;                    /* Tete ou LBA 24-27 */
  ideTransfType_t trfType;
  u16 *buf;                   /* Buffer */

  u8 command;                 /* Commande */
} ideParam_t;

typedef struct __ide_disksinf
{
  disk_t diskInf;
  struct __ide_disksinf *next;
} ideDisksInf_t;

/*****************************************************************************/
/**                           Declarations                                  **/
/*****************************************************************************/

#ifdef IDE_HD_DRIVER
extern diskID_t hdpCurrentId, hdCurrentId;
#endif
#ifdef IDE_CD_DRIVER
extern diskID_t cdCurrentId;
#endif

/* Ports correpondant aux 4 controleurs */
extern u16 ideAdapt[IDE_MAX_ADAPTERS];

/* Initialiser le driver */
u8                           /* Retour : code d'erreur */
    ideInitDriver(
      void
    );

/* Décharger le pilote */
u8                           /* Retour : code d'erreur */
    ideCleanupDriver(
      void
    );

/********************** Disques Logiques (partitions) ************************/

extern disksList_t ideDisksList;
extern ideDisksInf_t *ideDetectedDisksInf;

/* Obtenir des infos sur un disque */
u8                           /* Retour : code d'erreur */
    ideGetInf(
      diskID_t,              /* ID du disque */
      disk_t *               /* Structure pour le resultat */
    );

/* Initialiser un disque */
u8                           /* Retour : code d'erreur */
    ideInit( 
      diskID_t               /* ID du disque */
    );

/* Lire un disque */
u8                           /* Retour : code d'erreur */
    ideRead( 
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1e secteur a lire */
      u8 *                   /* Tampon memoire */
    );

/* Ecrire un disque */
u8                           /* Retour : code d'erreur */
    ideWrite( 
      diskID_t,              /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1e secteur a ecrire */
      u8 *                   /* Tampon memoire */
    );

/* Obtenir la taille en blocs de 512 octets d'un disque */
u8                           /* Retour : code d'erreur */
    ideGetSize(  
      diskID_t,              /* Disque */
      u32 *                  /* Emplacement pour le résultat */
    );

/* Bloquer le média */
u8                           /* Retour : code d'erreur */
    ideLock(
      diskID_t               /* Disque */
);

/* Débloquer le média */
u8                           /* Retour : code d'erreur */
    ideUnlock(
      diskID_t               /* Disque */
);

/* le m‚dia a-t-il chang‚ ? */
s8                           /* retour : -(code d'erreur), 0 (non), 1 (oui) */
    ideIsChanged(
      diskID_t               /* Disque */
    );

#ifdef IDE_HD_DRIVER
/* Obtenir les disques logiques presents sur un disque physique */
unsigned char                /* Retour : code d'erreur */
    ideGetLogicalDisks( 
      idePhysicalDiskID_t,   /* Disque */
      lbaAddress_t,          /* Adresse du MBR a lire */
      lbaAddress_t           /* MBR de base (pour les partitions etendues) */
    );
#endif

/********************** Disques Physiques ***********************************/

extern disksList_t idepDetectedDisks;
extern disk_t idepDetectedDisksInf[IDE_MAX_ADAPTERS*2];

/* Obtenir des infos sur un disque */
u8                           /* Retour : code d'erreur */
    idepGetInf(
      idePhysicalDiskID_t,   /* ID du disque */
      disk_t *,              /* Structure pour le resultat */
      u8,                    /* 0 ou 1 ; indique s'il faut interroger le */
                             /* disque ou regarder dans la liste */
      u8                     /* Protocole à utiliser pour interroger le disque */
    );

/* Initialiser un disque */
u8                           /* Retour : code d'erreur */
    idepInit(  
      idePhysicalDiskID_t    /* ID du disque */
    );

/* Lire un disque */
u8                           /* Retour : code d'erreur */
    idepRead(  
      idePhysicalDiskID_t,   /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1e secteur a lire */
      u8 *                   /* Tampon memoire */
    );

/* Ecrire un disque */
u8                           /* Retour : code d'erreur */
    idepWrite(  
      idePhysicalDiskID_t,   /* ID du disque */
      u32,                   /* Nombre de secteurs */
      lbaAddress_t,          /* Adresse LBA du 1e secteur a ecrire */
      u8 *                   /* Tampon memoire */
    );

/* Obtenir le disque selectionne d'un controleur */
s8                           /* Retour : disque ou -ERR */
    idepGetSelectedDisk(
      u8                     /* Numero du controleur */
    );

/* Changer le disque selectionne d'un controleur */
s8                           /* Retour : disque selectionne ou -ERR */
    idepSetSelectedDisk(
      u8,                    /* Numero du controleur */
      u8                     /* Disque MASTER ou SLAVE */
    );

#ifdef IDE_HD_DRIVER
/* Determiner les parametres d'adresse depuis l'adresse logique et le disque */
u8                           /* Retour : code d'erreur */
    idepGetAddressParam(
      idePhysicalDiskID_t,   /* ID du disque */
      lbaAddress_t,          /* Adresse logique */
      ideParam_t *           /* Parametres d'adresse */
    );
#endif

/* Envoyer une commande au controleur */
s16                          /* Retour : registre d'erreur ou -ERR */
    idepSendCommand(
      idePhysicalDiskID_t,   /* Disque */
      ideParam_t             /* Commande et parametres */
    );

#if defined(IDE_CD_DRIVER)
/* Envoyer une commande Packet */
u8                           /* Retour : code d'erreur */
    idepSendPacket(
      idePhysicalDiskID_t,   /* Disque */
      packet_t,              /* Packet */
      u8 *                   /* Buffer pour la réponse */
    );
#endif

/* Attendre que le controleur soit pret */
u8                           /* Retour : code d'erreur */
    idepWait(
      u8,                    /* Controleur */
      u8,                    /* Masque */
      u8                     /* Valeur OK */
    );

/* Detecter le protocole d'un disque */
s16                          /* 0 (ATA), 1 (ATAPI) ou -(code d'erreur) */
    idepGetProtocol(
      idePhysicalDiskID_t    /* Disque */
    );

#endif
