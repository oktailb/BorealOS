/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Pilotes bloc
*/

#include <drivers/block/block/block.h>

/* Initialisation */
s8 blockIsInitialized = 0;

/* Messages d'erreurs */
u8 *blockErrorsStr[]=
{
  "Aucune erreur",                            /* BLOCK_ERR_OK */
  "Donn�s incorrectes sur le disque",        /* BLOCK_ERR_DATA */
  "Commande interrompu ou non implémentée",   /* BLOCK_ERR_UNIMPLEMENTED */
  "Pas de secteur correspondant à l'adresse", /* BLOCK_ERR_LBA */
  "Disque inexistant ou non prêt",            /* BLOCK_ERR_DISK */
  "Nombre de secteurs trop grand",            /* BLOCK_ERR_SIZE */
  "Protection en écriture",                   /* BLOCK_ERR_WRITE */
  "Pilote non initialisé",                    /* BLOCK_ERR_UNINITIALIZED */
  "M�oire insuffisante",                     /* BLOCK_ERR_MEMORY */
  "Aucun pilote ne correspond au disque",     /* BLOCK_ERR_UNKNOWN_DRIVER */
  "Aucun disque amovible dans le lecteur",    /* BLOCK_ERR_NO_MEDIA */
  "Erreur ind�inie",                         /* BLOCK_ERR_UNKNOWN_ERROR */
  NULL
};

/* Cache */
blockDriverCache_t *blockCache;
u32 blockCacheSize=0;
u32 blockCacheCurrentId=0;

/* Liste des disques */
disksList_t blockDisksList;

/* Initialiser les pilotes */
u8 blockInitDriver(u32 cacheSize)
{
  u32 c;
#ifdef FLOPPY_DRIVER
  u8 f;
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  u8 i;
#endif
  disksListEntry_t *bdl,*pdl;

  if(cacheSize == 0) cacheSize = CACHE_DEFAULT_SIZE;
  blockCacheCurrentId = 0;
  blockCacheSize = cacheSize;
  if((blockCache = (blockDriverCache_t *)kMalloc(cacheSize * sizeof(blockDriverCache_t))) == NULL)
    return BLOCK_ERR_MEMORY;
  for(c=0;c<cacheSize;c++)
    blockCache[c].isUsed = 0;

#ifdef FLOPPY_DRIVER  
  f=floppyInitDriver();
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  i=ideInitDriver();
#endif

  /* Fusionner les listes disquettes et disque dur ide */
  blockDisksList.nDisks = 
#ifdef FLOPPY_DRIVER
    floppyDisksList.nDisks +
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
    ideDisksList.nDisks +
#endif
    0;

  /* Commencer par allouer la memoire necessaire */
  if(blockDisksList.nDisks)
    {
      if((blockDisksList.disks=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
	return BLOCK_ERR_MEMORY;
      blockDisksList.disks->next = NULL;
      for(bdl=blockDisksList.disks, c=1;c<blockDisksList.nDisks;c++,bdl=bdl->next)
	{
	  if((bdl->next=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
	    return BLOCK_ERR_MEMORY;
	  bdl->next->next = NULL;
	}
    }
  else blockDisksList.disks = NULL;

  /* Remplir la liste */
  bdl=blockDisksList.disks;

#ifdef FLOPPY_DRIVER
  for(pdl=floppyDisksList.disks;pdl!=NULL;pdl=pdl->next,bdl=bdl->next)
    bdl->disk = pdl->disk;
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  for(pdl=ideDisksList.disks;pdl!=NULL;pdl=pdl->next,bdl=bdl->next)
    bdl->disk = pdl->disk;
#endif

#ifdef FLOPPY_DRIVER
  if(f!=BLOCK_ERR_OK)
    return f;
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  if(i!=BLOCK_ERR_OK)
    return i;
#endif

  blockIsInitialized = 1;

  return BLOCK_ERR_OK;
}

/* D�harger le pilote */
u8 blockCleanupDriver(void)
{
  disksListEntry_t *de, *de2;
  u32 c;
#ifdef FLOPPY_DRIVER
  u8 f;
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  u8 i;
#endif

  c=blockSync();

#ifdef FLOPPY_DRIVER
  f = floppyCleanupDriver();
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  i = ideCleanupDriver();
#endif

  kFree(blockCache);
  for(de=blockDisksList.disks;de!=NULL;)
    {
      de2 = de->next;
      kFree(de);
      de = de2;
    }

  if(c != BLOCK_ERR_OK)
    return c;

#ifdef FLOPPY_DRIVER
  if(f != BLOCK_ERR_OK)
    return f;
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  if(i != BLOCK_ERR_OK)
    return i;
#endif

  return BLOCK_ERR_OK;
}

/* Synchronisation du cache */
u8 blockSync(void)
{
  u32 c;

  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  for(c=0;c<blockCacheSize;c++)
    {
      if(blockCache[c].isUsed)
	blockCache[c].isUsed = 0;
    }

  blockCacheCurrentId = 0;
  return BLOCK_ERR_OK;
}

/* Obtenir des informations sur un disque */
u8 blockGetInf(diskID_t id, disk_t *b)
{
  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyGetInf(id, b);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
  if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideGetInf(id, b);
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;
}

/* Initialiser un disque */
u8 blockInit(diskID_t id)
{
  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyInit(id);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideInit(id);
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;
}

/* Lire depuis un disque, priorite LOWlow */
u8 blockRead(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  return blockReadWithPriority(id, cnt, log, buffer, LOWlow);
}

/* Lire depuis un disque, priorite specifiee */
u8 blockReadWithPriority(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer, priorityLevel_t prior)
{
  u32 l, c;
  u8 r;
  u16 i;
  u8 (*read)(diskID_t, u32, lbaAddress_t, u8 *);
  blockDriverCache_t *bdc;

  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    read=floppyRead;
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    read=ideRead;
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;

  for(l=0;l<cnt;l++)
    {
      /* le secteur est-il dans le cache ? */
      for(c=0;c<blockCacheSize;c++)
	if(blockCache[c].isUsed && blockCache[c].dId == id && blockCache[c].log == log + l)
	  {
	    /* oui */
	    for(i=0;i<512;i++)
	      buffer[512 * l + i] = blockCache[c].sector[i];
	    blockCache[c].access ++;

	    break;
	  }
      if(c!=blockCacheSize)
	continue;

      /* non */
      if((r=blockNewCacheEntry(&bdc))!=BLOCK_ERR_OK)
	return r;
      r=read(id, 1, log + l, bdc->sector);
      
      if(r!=BLOCK_ERR_OK)
	{
	  bdc->isUsed = 0;
	  return r;
	}
      bdc->dId = id;
      bdc->log = log + l;
      bdc->priority = prior;
      
      for(i=0;i<512;i++)
	buffer[512 * l + i] = bdc->sector[i];
    }

  return BLOCK_ERR_OK;
}

/* Ecrire sur un disque directement, sans passer par le cache */
u8 blockWriteDirectly(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyWrite(id, cnt, log, buffer);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideWrite(id, cnt, log, buffer);
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;
}

/* Ecrire sur un disque, priorite LOWlow */
u8 blockWrite(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  return blockWriteWithPriority(id, cnt, log, buffer, LOWlow);
}

/* Ecrire sur un disque, priorite specifiee */
u8 blockWriteWithPriority(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer, priorityLevel_t prior)
{
  u32 l, c;
  u8 r, w;
  u16 i;
  blockDriverCache_t *bdc;

  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  /* On commence par &eacute;crire les secteurs sur le disque */
  w = blockWriteDirectly(id, cnt, log, buffer);

  /* Puis on place les secteurs dans le cache */
  for(l=0;l<cnt;l++)
    {
      /* le secteur est-il dans le cache ? */
      for(c=0;c<blockCacheSize;c++)
	if(blockCache[c].isUsed && blockCache[c].dId == id && blockCache[c].log == log + l)
	  {
	    /* oui */
	    for(i=0;i<512;i++)
	      blockCache[c].sector[i] = buffer[512 * l + i];
	    blockCache[c].access ++;

	    break;
	  }
      if(c!=blockCacheSize)
	continue;

      /* non, alors on l'y place */
      if((r=blockNewCacheEntry(&bdc))!=BLOCK_ERR_OK)
	return r;
      bdc->dId = id;
      bdc->log = log + l;
      bdc->priority = prior;
      for(i=0;i<512;i++)
	bdc->sector[i] = buffer[512 * l + i];
    }

  return w;
}

/* Trouver une nouvelle entree dans le cache */
u8 blockNewCacheEntry(blockDriverCache_t **ce)
{
  u32 c,d,i;
  u8 r;
  priorityLevel_t p;
  u16 a;

  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  /* Faut-il synchroniser ? */
  if(blockCacheCurrentId == CACHE_AUTO_SYNC)
    if((r=blockSync())!=BLOCK_ERR_OK)
      return r;
      
  /* rechercher s'il existe une place libre */
  for(c=0;c<blockCacheSize;c++)
    if(!blockCache[c].isUsed)
      {
	/* Place libre */
	blockCache[c].isUsed = 1;
	blockCache[c].id = blockCacheCurrentId ++;
	blockCache[c].access = 1;

	(*ce) = &blockCache[c];

	return BLOCK_ERR_OK;
      }

  /* il faut faire une place */

  /* Commencer par trouver la plus basse priorite (ou plutot la plus haute car HIGHhigh correspond a 0 */
  for(c=0, p=HIGHhigh;c<blockCacheSize;c++)
    if(blockCache[c].priority > p) p = blockCache[c].priority;

  /* Trouver maintenant le plus petit nombre d'acces */
  for(c=0, a=0xffff;c<blockCacheSize;c++)
    if(blockCache[c].priority == p && blockCache[c].access < a) a = blockCache[c].access;

  /* Et enfin l'entree la plus ancienne (ID le plus petit) */
  for(i=blockCacheCurrentId, c=0, d=0;c<blockCacheSize;c++)
    if(blockCache[c].priority == p && blockCache[c].access == a && blockCache[c].id < i) { i = blockCache[c].id; d=c; }

  /* Supprimer cette entree */

  /* Place libre */
  blockCache[d].isUsed = 1;
  blockCache[d].id = blockCacheCurrentId ++;
  blockCache[d].access = 1;

  (*ce) = &blockCache[d];

  return BLOCK_ERR_OK;
}

/* V�ifier l'existance d'un disque */
u8 blockDiskExists(diskID_t id)
{
  disksListEntry_t *de;

  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  for(de=blockDisksList.disks;de!=NULL;de=de->next)
    if(de->disk == id)
      return 1;

  return 0;
}

/* Obtenir la taille en blocs de 512 octets d'un disque */
u8 blockGetSize(diskID_t id, u32 *size)
{
  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyGetSize(id, size);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideGetSize(id, size);
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;
}

u8 blockLock(diskID_t id)
{
  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyLock(id);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideLock(id);
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;
}

u8 blockUnlock(diskID_t id)
{
  if(blockIsInitialized == 0)
    return BLOCK_ERR_UNINITIALIZED;

  if(!blockDiskExists(id))
    return BLOCK_ERR_DISK;

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyUnlock(id);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideUnlock(id);
  else
#endif
    return BLOCK_ERR_UNKNOWN_DRIVER;
}

s8 blockIsChanged(diskID_t id)
{
  if(blockIsInitialized == 0)
    return -(BLOCK_ERR_UNINITIALIZED);

  if(!blockDiskExists(id))
    return -(BLOCK_ERR_DISK);

#ifdef FLOPPY_DRIVER
  if((id>>12) == 0)
    return floppyIsChanged(id);
  else
#endif
#if defined(IDE_HD_DRIVER) || defined(IDE_CD_DRIVER)
 if((id>>12) == 1 || (id>>12) == 2 || (id>>12) == 3)
    return ideIsChanged(id);
  else
#endif
    return -(BLOCK_ERR_UNKNOWN_DRIVER);
}
