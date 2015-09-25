/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Pilote IDE
*/

#include "ide.h"

u16 ideAdapt[] = { 0x1f0, 0x170, 0xf0, 0x70 };

disksList_t ideDisksList;
ideDisksInf_t *ideDetectedDisksInf;
disksList_t idepDetectedDisks;
disk_t idepDetectedDisksInf[IDE_MAX_ADAPTERS*2];

#ifdef IDE_HD_DRIVER
diskID_t hdpCurrentId=0x1000;  /* Partitions de disques durs */
diskID_t hdCurrentId=0x2000;   /* Disques durs */
#endif

#ifdef IDE_CD_DRIVER
diskID_t cdCurrentId=0x3000;   /* CD-ROMs */
#endif

/* Inititaliser le pilote */
u8 ideInitDriver(void)
{
  // Obtenir la liste des disques
  u8 adapt;
  u8 c;
  disksListEntry_t *de, *de2;
  ideDisksInf_t *di;
  diskID_t i;
  disk_t pdi;
  s16 proto;

  /* Detecter les disques physiques */

  idepDetectedDisks.nDisks = 0;

  for(adapt=1;adapt<(IDE_MAX_ADAPTERS + 1);adapt++)
    {
      // detecter la presence du controleur
      if((proto=idepGetProtocol(adapt<<4 | IDE_MASTER)) >= 0)
	{
	  if(idepGetInf(adapt<<4 | IDE_MASTER, &idepDetectedDisksInf[idepDetectedDisks.nDisks], 0, proto)==BLOCK_ERR_OK)
	    {
	      if(idepDetectedDisks.nDisks)
		{
		  for(de=idepDetectedDisks.disks;de->next!=NULL;de=de->next)
		    ;
		  if((de->next=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
		    return BLOCK_ERR_MEMORY;
		  de->next->disk = idepDetectedDisksInf[idepDetectedDisks.nDisks].id;
		  de->next->next = NULL;
		  idepDetectedDisks.nDisks++;
		}
	      else
		{
		  if((idepDetectedDisks.disks=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
		    return BLOCK_ERR_MEMORY;
		  idepDetectedDisks.disks->disk = idepDetectedDisksInf[idepDetectedDisks.nDisks].id;
		  idepDetectedDisks.disks->next = NULL;
		  idepDetectedDisks.nDisks++;
		}
	    }
	}

      if((proto=idepGetProtocol(adapt<<4 | IDE_SLAVE)) >= 0)
	{
	  if(idepGetInf(adapt<<4 | IDE_SLAVE, &idepDetectedDisksInf[idepDetectedDisks.nDisks],0, proto)==BLOCK_ERR_OK)
	    {
	      if(idepDetectedDisks.nDisks)
		{
		  for(de=idepDetectedDisks.disks;de->next!=NULL;de=de->next)
		    ;
		  if((de->next=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
		    return BLOCK_ERR_MEMORY;
		  
		  de->next->disk = idepDetectedDisksInf[idepDetectedDisks.nDisks].id;
		  de->next->next = NULL;
		  idepDetectedDisks.nDisks++;
		}
	      else
		{
		  if((idepDetectedDisks.disks=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
		    return BLOCK_ERR_MEMORY;
		  idepDetectedDisks.disks->disk = idepDetectedDisksInf[idepDetectedDisks.nDisks].id;
		  idepDetectedDisks.disks->next = NULL;
		  idepDetectedDisks.nDisks++;
		}
	    }
	}
    }

  // detecter les disques logiques
  ideDisksList.nDisks = 0;
  for(de=idepDetectedDisks.disks;de!=NULL;de=de->next)
    {
      // ajouter d'abord le disque physique
      idepGetInf(de->disk, &pdi, 1, 0);

#if defined(IDE_HD_DRIVER) && !defined(IDE_CD_DRIVER)
      if(pdi.type != ideHd)
	continue;
#endif
#if defined(IDE_CD_DRIVER) && !defined(IDE_HD_DRIVER)
      if(pdi.type != ideCd)
	continue;
#endif

      if(ideDisksList.nDisks)
	{
	  for(de2=ideDisksList.disks;de2->next!=NULL;de2=de2->next)
	    ;
	  if((de2->next=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
	    return BLOCK_ERR_MEMORY;

#ifdef IDE_HD_DRIVER
	  if(pdi.type == ideHd)
	    {
	      de2->next->disk = hdCurrentId ++;
	      i = hdCurrentId - 1;
	    }
#endif
#ifdef IDE_CD_DRIVER
	  if(pdi.type == ideCd)
	    {
	      de2->next->disk = cdCurrentId ++;
	      i = cdCurrentId - 1;
	    }
#endif

	  de2->next->next = NULL;
	  ideDisksList.nDisks++;
	  
	  for(di=ideDetectedDisksInf;di->next!=NULL;di=di->next)
		;
	  if((di->next=(ideDisksInf_t*)kMalloc(sizeof(ideDisksInf_t)))==NULL)
	    return BLOCK_ERR_MEMORY;
	  di=di->next;
	}
      else
	{
	  if((ideDisksList.disks=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
	    return BLOCK_ERR_MEMORY;
#ifdef IDE_HD_DRIVER
	  if(pdi.type == ideHd)
	    {
	      ideDisksList.disks->disk = hdCurrentId ++;
	      i = hdCurrentId - 1;
	    }
#endif
#ifdef IDE_CD_DRIVER
	  if(pdi.type == ideCd)
	    {
	      ideDisksList.disks->disk = cdCurrentId ++;
	      i = cdCurrentId - 1;
	    }
#endif
	  ideDisksList.disks->next = NULL;
	  ideDisksList.nDisks++;
	      
	  if((ideDetectedDisksInf=(ideDisksInf_t*)kMalloc(sizeof(ideDisksInf_t)))==NULL)
	    return BLOCK_ERR_MEMORY;
	  di = ideDetectedDisksInf;
	}

      di->next = NULL;
      di->diskInf.id = i;
      di->diskInf.nSects = pdi.nSects;
      di->diskInf.type = pdi.type;
      for(c=0;c<MAX_MODEL_STR;c++)
	di->diskInf.model[c] = pdi.model[c];
      for(c=0;c<MAX_SERIAL_STR;c++)
	di->diskInf.serial[c] = pdi.serial[c];

#ifdef IDE_HD_DRIVER
      if(di->diskInf.type == ideHd)
	{
	  di->diskInf.spec.ideHd.lba = pdi.spec.ideHd.lba;
	  di->diskInf.spec.ideHd.start = 0;
	  di->diskInf.spec.ideHd.pId = de->disk;
	  di->diskInf.spec.ideHd.heads = pdi.spec.ideHd.heads;
	  di->diskInf.spec.ideHd.cyls = pdi.spec.ideHd.cyls;
	  di->diskInf.spec.ideHd.sectPerCyl = pdi.spec.ideHd.sectPerCyl;
	  di->diskInf.spec.ideHd.partBIOS = 0;

	  /* Fonctions supportées */
	  di->diskInf.features.init = TRUE;
	  di->diskInf.features.read = TRUE;
	  di->diskInf.features.write = TRUE;
	  di->diskInf.features.getInf = TRUE;
	  di->diskInf.features.getSize = TRUE;
	  di->diskInf.features.lock = FALSE;
	  di->diskInf.features.unlock = FALSE;
	  di->diskInf.features.isChanged = FALSE;
	  
	  ideGetLogicalDisks(de->disk, 0, 0);
	}
#endif
#ifdef IDE_CD_DRIVER
      if(di->diskInf.type == ideCd)
	{
	  di->diskInf.spec.ideCd.pId = de->disk;

	  /* Fonctions supportées */
	  di->diskInf.features.init = TRUE;
	  di->diskInf.features.read = TRUE;
	  di->diskInf.features.write = FALSE;
	  di->diskInf.features.getInf = TRUE;
	  di->diskInf.features.getSize = TRUE;
	  di->diskInf.features.lock = TRUE;
	  di->diskInf.features.unlock = TRUE;
	  di->diskInf.features.isChanged = FALSE;
	}
#endif
    }

  return BLOCK_ERR_OK;
}

/* Décharger le pilote */
u8 ideCleanupDriver(void)
{
  disksListEntry_t *de, *de2;
  ideDisksInf_t *di, *di2;

  for(de=ideDisksList.disks;de!=NULL;)
    {
      de2 = de->next;
      kFree(de);
      de = de2;
    }

  for(di=ideDetectedDisksInf;di!=NULL;)
    {
      di2 = di->next;
      kFree(di);
      di = di2;
    }

  for(de=idepDetectedDisks.disks;de!=NULL;)
    {
      de2 = de->next;
      kFree(de);
      de = de2;
    }

  return BLOCK_ERR_OK;
}

/* Obtenir des infos sur un disque (ID logique) */
u8 ideGetInf(diskID_t id, disk_t *disk)
{
  u8 d;
  ideDisksInf_t *di;

  // chercher le disque dans la liste
  for(di=ideDetectedDisksInf;di!=NULL;di=di->next)
    if(di->diskInf.id==id)
      break;
  if(di==NULL)
    return BLOCK_ERR_DISK;

  disk->id = di->diskInf.id;
  disk->nSects = di->diskInf.nSects;
  disk->type=di->diskInf.type;
  for(d=0;d<41;d++)
    disk->model[d] = di->diskInf.model[d];
  for(d=0;d<21;d++)
    disk->serial[d] = di->diskInf.serial[d];

  /* Fonctions supportées */
  disk->features.init = di->diskInf.features.init;
  disk->features.read = di->diskInf.features.read;
  disk->features.write = di->diskInf.features.write;
  disk->features.getInf = di->diskInf.features.getInf;
  disk->features.getSize = di->diskInf.features.getSize;
  disk->features.lock = di->diskInf.features.lock;
  disk->features.unlock = di->diskInf.features.unlock;
  disk->features.isChanged = di->diskInf.features.isChanged;

#ifdef IDE_HD_DRIVER
  if(di->diskInf.type == ideHd)
    {
      disk->spec.ideHd.lba = di->diskInf.spec.ideHd.lba;
      disk->spec.ideHd.start = di->diskInf.spec.ideHd.start;
      disk->spec.ideHd.pId = di->diskInf.spec.ideHd.pId;
      disk->spec.ideHd.heads = di->diskInf.spec.ideHd.heads;
      disk->spec.ideHd.cyls = di->diskInf.spec.ideHd.cyls;
      disk->spec.ideHd.sectPerCyl = di->diskInf.spec.ideHd.sectPerCyl;
      disk->spec.ideHd.partBIOS = di->diskInf.spec.ideHd.partBIOS;
    }
#endif
#ifdef IDE_CD_DRIVER
  if(di->diskInf.type == ideCd)
    {
      disk->spec.ideCd.pId = di->diskInf.spec.ideCd.pId;
    }
#endif

  return BLOCK_ERR_OK;
}

/* Initialiser un disque (ID logique) */
u8 ideInit(diskID_t id)
{
  u8 c;
  disk_t d;

  if((c=ideGetInf(id, &d))!=BLOCK_ERR_OK)
    return c;

#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    return idepInit(d.spec.ideHd.pId);
#endif
#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    return idepInit(d.spec.ideCd.pId);
#endif

  return 0; /* Pour éviter les warnings mais ne devrait pas être exécuté */
}

/* Obtenir la taille en blocs de 512 octets d'un disque */
u8 ideGetSize(diskID_t id, u32 *size)
{
  disk_t d;

  ideGetInf(id, &d);

#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    {
      (*size) = d.nSects;
      return BLOCK_ERR_OK;
    }
#endif
#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      packet_t pckt;
      u8 resp[8];
      u8 error;

      packetCdReadCapacity(&pckt);

      error = idepSendPacket(d.spec.ideCd.pId, pckt, resp);

      if(error == BLOCK_ERR_OK)
	*size = ((resp[0]<<24 | resp[1]<<16 | resp[2]<<8 | resp[3]) + 1) * 4;
      return error;
    }
#endif

  return 0; /* Présent pour éviter les warnings, ne devrait pas être exécuté */
}

/* Bloquer */
u8 ideLock(diskID_t id)
{
  disk_t d;

  ideGetInf(id, &d);

#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    return BLOCK_ERR_UNIMPLEMENTED;
#endif

#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      packet_t pckt;

      packetPreventAllowMediumRemoval(FALSE, &pckt);
      return idepSendPacket(d.spec.ideCd.pId, pckt, NULL);
    }
#endif

  return BLOCK_ERR_UNIMPLEMENTED;
}

/* Débloquer */
u8 ideUnlock(diskID_t id)
{
  disk_t d;

  ideGetInf(id, &d);

#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    return BLOCK_ERR_UNIMPLEMENTED;
#endif

#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      packet_t pckt;
      
      packetPreventAllowMediumRemoval(TRUE, &pckt);
      return idepSendPacket(d.spec.ideCd.pId, pckt, NULL);
    }
#endif

  return BLOCK_ERR_UNIMPLEMENTED;
}

/* Le m‚dia a-t-il chang‚ ? */
s8 ideIsChanged(diskID_t id)
{
  return -(BLOCK_ERR_UNIMPLEMENTED);
}

/* Lire depuis un disque (ID logique) */
u8 ideRead(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buf)
{
  disk_t d;
  u8 c;

  if((c=ideGetInf(id, &d))!=BLOCK_ERR_OK)
    return c;
  
#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    {
      if(log + cnt > d.nSects)
	return BLOCK_ERR_LBA;

      return idepRead(d.spec.ideHd.pId, cnt, log+d.spec.ideHd.start, buf);
    }
#endif
#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      u32 size;
      u8 c;

      if((c=ideGetSize(d.id, &size))!=BLOCK_ERR_OK)
	return c;

      if(log + cnt > size)
	return BLOCK_ERR_LBA;

      return idepRead(d.spec.ideCd.pId, cnt, log, buf);
    }
#endif

  return 0; /* Présent pour éviter les warnings, ne devrait pas être exécuté */
}

/* Ecrire sur un disque (ID logique) */
u8 ideWrite(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buf)
{
  disk_t d;
  u8 c;

  if((c=ideGetInf(id, &d))!=BLOCK_ERR_OK)
    return c;
  
#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    {
      if(log + cnt > d.nSects)
	return BLOCK_ERR_LBA;

      return idepWrite(d.spec.ideHd.pId, cnt, log + d.spec.ideHd.start, buf);
    }
#endif
#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      u32 size;
      u8 c;

      if((c=ideGetSize(d.id, &size))!=BLOCK_ERR_OK)
	return c;
      if(log + cnt > size)
	return BLOCK_ERR_LBA;

      return idepWrite(d.spec.ideCd.pId, cnt, log, buf);
    }
#endif

  return 0; /* Présent pour éviter les warnings, ne devrait pas être exécuté */
}

#ifdef IDE_HD_DRIVER
/* Obtenir les disques logiques presents sur un disque physique */
u8 ideGetLogicalDisks(idePhysicalDiskID_t id, lbaAddress_t mbr, lbaAddress_t baseMBR)
{
  /*
    Note : pour les partitions etendues, l'adresse logique du premier secteur est calculee par rapport au 1e secteur :
    - de la 1e partition dans le cas d'une 'sous-partition' etendue
    - de la 'sous-partition' dans le cas d'une partition logique
  */

  u8 buf[512];
  u8 c, d;
  diskID_t i;
  disksListEntry_t *de;
  ideDisksInf_t *di;
  disk_t pdi;
  
  if((c=idepRead(id, 1, mbr, buf))!=BLOCK_ERR_OK)
    return c;

  for(c=0;c<4;c++)
    {
      switch(buf[0x1be + c*0x10 + 4])
	{
	case 0:        /* Aucune partition */
	  break;
	case 5:        /* Etendue DOS */
	  ideGetLogicalDisks(id, baseMBR + (buf[0x1be + c*0x10 + 11] << 24 | buf[0x1be + c*0x10 + 10] << 16 | buf[0x1be + c*0x10 + 9] << 8 | buf[0x1be + c*0x10 + 8]), baseMBR==0?(buf[0x1be + c*0x10 + 11] << 24 | buf[0x1be + c*0x10 + 10] << 16 | buf[0x1be + c*0x10 + 9] << 8 | buf[0x1be + c*0x10 + 8]):baseMBR);
	  break;
	default:       /* Partition normale */
	  if(ideDisksList.nDisks)
	    {
	      for(de=ideDisksList.disks;de->next!=NULL;de=de->next)
		;
	      if((de->next=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
		return BLOCK_ERR_MEMORY;
	      de->next->disk = hdpCurrentId ++;
	      de->next->next = NULL;
	      ideDisksList.nDisks++;

	      for(di=ideDetectedDisksInf;di->next!=NULL;di=di->next)
		;
	      if((di->next=(ideDisksInf_t*)kMalloc(sizeof(ideDisksInf_t)))==NULL)
		return BLOCK_ERR_MEMORY;
	      di=di->next;
	    }
	  else
	    {
	      if((ideDisksList.disks=(disksListEntry_t*)kMalloc(sizeof(disksListEntry_t)))==NULL)
		return BLOCK_ERR_MEMORY;
	      ideDisksList.disks->disk = hdpCurrentId ++;
	      ideDisksList.disks->next = NULL;
	      ideDisksList.nDisks++;

	      if((ideDetectedDisksInf=(ideDisksInf_t*)kMalloc(sizeof(ideDisksInf_t)))==NULL)
		return BLOCK_ERR_MEMORY;
	      di = ideDetectedDisksInf;
	    }
	  i = hdpCurrentId - 1;
	  idepGetInf(id, &pdi, 1, 0);
	  di->next = NULL;
	  di->diskInf.id = i;
	  di->diskInf.nSects = buf[0x1be + c*0x10 + 15] << 24 | buf[0x1be + c*0x10 + 14] << 16 | buf[0x1be + c*0x10 + 13] << 8 | buf[0x1be + c*0x10 + 12];
	  di->diskInf.type = ideHd;
	  for(d=0;d<MAX_MODEL_STR;d++)
	    di->diskInf.model[d] = pdi.model[d];
	  for(d=0;d<MAX_SERIAL_STR;d++)
	    di->diskInf.serial[d] = pdi.serial[d];
	  di->diskInf.spec.ideHd.lba = pdi.spec.ideHd.lba;
	  di->diskInf.spec.ideHd.start = mbr + (buf[0x1be + c*0x10 + 11] << 24 | buf[0x1be + c*0x10 + 10] << 16 | buf[0x1be + c*0x10 + 9] << 8 | buf[0x1be + c*0x10 + 8]);
	  di->diskInf.spec.ideHd.pId = id;
	  di->diskInf.spec.ideHd.heads = pdi.spec.ideHd.heads;
	  di->diskInf.spec.ideHd.cyls = pdi.spec.ideHd.cyls;
	  di->diskInf.spec.ideHd.sectPerCyl = pdi.spec.ideHd.sectPerCyl;
	  di->diskInf.spec.ideHd.partBIOS = buf[0x1be + c*0x10 + 4];

	  /* Fonctions supportées */
	  di->diskInf.features.init = TRUE;
	  di->diskInf.features.read = TRUE;
	  di->diskInf.features.write = TRUE;
	  di->diskInf.features.getInf = TRUE;
	  di->diskInf.features.getSize = TRUE;
	  di->diskInf.features.lock = FALSE;
	  di->diskInf.features.unlock = FALSE;
	  di->diskInf.features.isChanged = FALSE;
	}
   }
  return BLOCK_ERR_OK;
}
#endif

/* Obtenir le disque physique selectionne sur un controleur */
s8 idepGetSelectedDisk(u8 adapt)
{
  u16 port;
  u8 val;

  port=ideAdapt[adapt-1] + IDE_HEADDSK;
  __asm__("inb %%dx, %%al\n" : "=a" (val) : "d" (port));

  return ((val & 0x10) >> 4);
}

/* Selectionner un disque physique sur un controleur */
s8 idepSetSelectedDisk(u8 adapt, u8 disk)
{
  u16 port;
  u8 val;

  port=ideAdapt[adapt-1] + IDE_HEADDSK;
  __asm__("inb %%dx, %%al\n" : "=a" (val) : "d" (port));

  if(disk == IDE_MASTER) val = val & 0xef;
  else val = val | 0x10;

  __asm__("outb %%al, %%dx\n" : : "a" (val), "d" (port));

  return idepGetSelectedDisk(adapt);
}

/* Obtenir des infos sur un disque physique */
u8 idepGetInf(idePhysicalDiskID_t id, disk_t *disk, u8 method, u8 proto)
{
  u16 buf[0x100];
  u16 val16;
  s16 c, d;
  u8 bufc[41];
  u32 l;
  ideParam_t p;

  if(method == 0 && proto == ATA_PROTOCOL)
    {
#ifndef IDE_HD_DRIVER
      return BLOCK_ERR_UNKNOWN_DRIVER;
#endif
    }

  if(method == 0 && proto == ATAPI_PROTOCOL)
    {
#ifndef IDE_CD_DRIVER
      return BLOCK_ERR_UNKNOWN_DRIVER;
#endif
    }

  if(method == 0)  /* interroger le disque */
    {
      p.nSects = 1;
      p.sector = 0;
      p.cylLow = 0;
      p.cylHigh = 0;
      p.head = 0;
      p.adrType=chs;
      p.trfType = itt_read;
      p.buf = buf;
      p.command = proto==ATA_PROTOCOL?IDE_COMM_GETINF:IDE_COMM_GETINF_PACKET;
      
      val16=idepSendCommand(id, p);
      if(((s16)val16)<0)
	return (s16)val16;
      
      if((val16 & 4) == 4)
	return BLOCK_ERR_UNIMPLEMENTED; // aucune autre erreur ne peut survenir pour
      // cette commande
      
      // id
      disk->id = id;
      
      // model
      for(c=0,d=0;c<40;c+=2,d+=2)
	{
	  bufc[c] = buf[0x1b + c/2] >> 8;
	  bufc[c+1] = buf[0x1b + c/2] & 0xff;
	}
      for(c=39;bufc[c]<=32;c--)
	;
      bufc[c+1]=0;
      for(c=0;bufc[c]<=32;c++)
	;
      for(d=0;c<41;d++,c++)
	disk->model[d]=bufc[c];

      // serial
      for(c=0,d=0;c<20;c+=2,d+=2)
	{
	  bufc[c] = buf[0xa + c/2] >> 8;
	  bufc[c+1] = buf[0xa + c/2] & 0xff;
	}
      for(c=19;bufc[c]<=32;c--)
	;
      bufc[c+1]=0;
      for(c=0;bufc[c]<=32;c++)
	;
      for(d=0;c<21;d++,c++)
	disk->serial[d]=bufc[c];
      
      if(proto == ATA_PROTOCOL)
	{
#ifdef IDE_HD_DRIVER
	  // nombre de secteurs
	  disk->nSects = buf[0x39] | buf[0x3a] << 16;
      
	  // type
	  disk->type=ideHd;
	
	  // specifique au type
	  l = buf[0x3c] | buf[0x3d] << 16;
	  if(l==disk->nSects) disk->spec.ideHd.lba = 1;
	  else disk->spec.ideHd.lba = 0;
      
	  disk->spec.ideHd.heads = buf[0x37];
	  disk->spec.ideHd.cyls = buf[0x36];
	  disk->spec.ideHd.sectPerCyl = buf[0x38];
#endif
	}
      else
	{
#ifdef IDE_CD_DRIVER
	  // nombre de secteurs
	  disk->nSects = 0; // 0 pour les disques amovibles

	  // type
	  if((buf[0]&0xf00)>>8 == DEVICE_ATAPI_CDROM)
	    disk->type=ideCd;
	  else return BLOCK_ERR_DISK;
	  // specifique au type
#endif
	}

      return BLOCK_ERR_OK; // pas d'erreur
    }
  else   /* consulter la liste */
    {
      for(c=0;c<IDE_MAX_ADAPTERS*2;c++)
	{
	  if(idepDetectedDisksInf[c].id == id)
	    {
	      for(d=0;d<sizeof(disk_t);d++)
		((u8 *)disk)[d] = ((u8 *)(&idepDetectedDisksInf[c]))[d];
	      return BLOCK_ERR_OK;
	    }
	}
      return BLOCK_ERR_DISK;
    }

  return 0; /* Présent pour éviter les warnings, ne devrait pas être exécuté */
}

/* Initialiser un disque physique */
u8 idepInit(idePhysicalDiskID_t id)
{
  u16 port;
  u8 val;
  u32 l;

  idepSetSelectedDisk(id>>4, id&0xf);

  port = ideAdapt[(id>>4)-1] + IDE_CTRL;
  val = 0xc;
  __asm__("outb %%al, %%dx" : : "a" (val), "d" (port));

  port = port - IDE_CTRL + IDE_STAT2;
  /* 12000 * 400ns = 4.8ms */
  for(l=0;l<12000;l++)
    L400NS(port);
  port = port - IDE_STAT2 + IDE_CTRL;

  val = 0x8;
  __asm__("outb %%al, %%dx" : : "a" (val), "d" (port));

  port = port - IDE_CTRL + IDE_STAT2;
  for(l=0;l<12000;l++)
    L400NS(port);
  port = port - IDE_STAT2 + IDE_CTRL;

  idepSetSelectedDisk(id>>4, id&0xf);

  return BLOCK_ERR_OK;
}

/* Lire depuis un disque physique */
u8 idepRead(idePhysicalDiskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  ideParam_t p;
  s16 res;
  disk_t d;

  idepGetInf(id, &d, 1, 0);

#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    {
      if(cnt == 0)
	return BLOCK_ERR_SIZE;
      if(cnt > 256)
	return BLOCK_ERR_SIZE;
      if(cnt == 256)
	cnt = 0;
      
      p.nSects = cnt;
      idepGetAddressParam(id, log, &p);
      p.trfType = itt_read;
      p.buf = (u16 *)buffer;
      p.command = IDE_COMM_READ;
      
      res = idepSendCommand(id, p);
      if((res & 0x40) == 0x40 || (res & 0x1) == 1)
	return BLOCK_ERR_DATA;
      if((res & 0x10) == 0x10)
	return BLOCK_ERR_LBA;
    }
#endif
#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      /* lecture par atapi */
      packet_t pckt;
      u8 *resp = (u8*)kMalloc(2048 * (cnt/4 + cnt%4 == 0 ? 0 : 1));
      u8 error;
      u32 c;

      if(resp == NULL)
	return BLOCK_ERR_MEMORY;

      packetCdRead(log/4, cnt/4 + (cnt%4 == 0 ? 0 : 1), &pckt);
      error = idepSendPacket(id, pckt, resp);

      if(error == BLOCK_ERR_OK)
	{
	  /* Copier les secteurs demand‚s dans buffer */
	  for(c=0;c<512 * cnt;c++)
	    buffer[c] = resp[(log%4)*512 + c];
	}

      kFree(resp);
      return error;
    }
#endif

  return BLOCK_ERR_OK;
};

/* Ecrire sur un disque physique */
u8 idepWrite(idePhysicalDiskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  ideParam_t p;
  s16 res;
  disk_t d;

  idepGetInf(id, &d, 1, 0);

#ifdef IDE_HD_DRIVER
  if(d.type == ideHd)
    {
      if(cnt == 0)
	return BLOCK_ERR_SIZE;
      if(cnt > 256)
	return BLOCK_ERR_SIZE;
      if(cnt == 256)
	cnt = 0;

      p.nSects = cnt;
      idepGetAddressParam(id, log, &p);
      p.trfType = itt_write;
      p.buf = (u16 *)buffer;
      p.command = IDE_COMM_WRITE;

      res = idepSendCommand(id, p);
      if((res & 0x40) == 0x40 || (res & 0x1) == 1)
	return BLOCK_ERR_DATA;
      if((res & 0x10) == 0x10)
	return BLOCK_ERR_LBA;
    }
#endif
#ifdef IDE_CD_DRIVER
  if(d.type == ideCd)
    {
      /* ecriture atapi */
      /* Actuellement non support‚e */
      /* On renvoie write protect car ce programme est un pilote de lecteur et non de graveur */
      return BLOCK_ERR_WRITE;
    }
#endif

  return BLOCK_ERR_OK;
};

#ifdef IDE_HD_DRIVER
/* Obtenir les parametres d'adressage en fonction de l'adresse logique et des caracteristiques du disque physique */
u8 idepGetAddressParam(idePhysicalDiskID_t id, lbaAddress_t log, ideParam_t *param)
{
  disk_t d;
  u8 c;

  // Infos sur le disque
  if((c=idepGetInf(id, &d, 1, 0))!=BLOCK_ERR_OK)
    return c;

  if(d.spec.ideHd.lba)
    {
      param->adrType = lba;
      param->sector = log & 0xff;
      param->cylLow = (log & (0xff << 8)) >> 8;
      param->cylHigh = (log & (0xff << 16)) >> 16;
      param->head = (log & (0xf << 24)) >> 24;
    }
  else
    {
      param->adrType = chs;
      param->head = (log / d.spec.ideHd.sectPerCyl) % d.spec.ideHd.heads;
      param->sector = 1 + (log % d.spec.ideHd.sectPerCyl);
      param->cylLow = (log / (d.spec.ideHd.sectPerCyl * d.spec.ideHd.heads)) & 0xff;
      param->cylHigh = (log / (d.spec.ideHd.sectPerCyl * d.spec.ideHd.heads)) >> 8;
    }

  return BLOCK_ERR_OK;
}
#endif

/* Envoyer une commande a un controleur */
s16 idepSendCommand(idePhysicalDiskID_t id, ideParam_t param)
{
  u8 c;
  u16 port, val16;
  u32 i;

  // ecrire tous les parametres
  port = ideAdapt[(id>>4)-1] + IDE_COUNT;
  __asm__("outb %%al, %%dx\n" : : "a" (param.nSects), "d" (port));

  port = port - IDE_COUNT + IDE_SECT;
  __asm__("outb %%al, %%dx\n" : : "a" (param.sector), "d" (port));
  port = port - IDE_SECT + IDE_CYLLOW;
  __asm__("outb %%al, %%dx\n" : : "a" (param.cylLow), "d" (port));
  port = port - IDE_CYLLOW + IDE_CYLHIGH;
  __asm__("outb %%al, %%dx\n" : : "a" (param.cylHigh), "d" (port));
  port = port - IDE_CYLHIGH + IDE_HEADDSK;
  c = 0xa0;
  if(param.adrType==lba) c |= 0x40;
  c |= (id&0xf)<<4;
  c |= param.head;
  __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));

  port = port - IDE_HEADDSK + IDE_STAT2;
  L400NS(port);
  port = port - IDE_STAT2 + IDE_HEADDSK;

  // ecrire la commande
  port = port - IDE_HEADDSK + IDE_COMMAND;
  __asm__("outb %%al, %%dx\n" : : "a" (param.command), "d" (port));

  /* Attendre 400ns avant de lire l'état du contrôleur */
  port = port - IDE_COMMAND + IDE_STAT2;
  L400NS(port);
  port = port - IDE_STAT2 + IDE_COMMAND;

  // attendre que le controleur soit pret
  if((c=idepWait(id>>4, 0x80, 0x0))!=BLOCK_ERR_OK)
    return -c;
    
  // effectuer le transfert s'il y a lieu.
  if(param.trfType != itt_no)
    {
      // attendre que le disque soit pret a transferer des donnees
      if((c=idepWait(id>>4, 0xd8, 0x58))!=BLOCK_ERR_OK)
	return -c;

      // lire param.nSects*256 mots d'info.
      port = port - IDE_STAT + IDE_DATA;
      for(i=0;i<0x100*param.nSects;i++)
	{
	  port = port - IDE_DATA + IDE_STAT2;
	  L400NS(port);
	  port = port - IDE_STAT2 + IDE_DATA;

	  if(param.trfType==itt_read)
	    {
	      __asm__("inw %%dx, %%ax\n"
		      : "=a" (val16)
		      : "d" (port));
	      
	      param.buf[i] = val16;
	    }
	  else
	    {
	      val16 = param.buf[i];

	      __asm__("outw %%ax, %%dx\n"
		      :
		      : "a" (val16), "d" (port));
	    }
	}
      port = port - IDE_DATA + IDE_COMMAND;
    }

  // attendre que le controleur ne soit plus occupe
  if((c=idepWait(id>>4, 0x80, 0x0))!=BLOCK_ERR_OK)
    return -c;

  // renvoyer le registre d'erreur
  port = port - IDE_COMMAND + IDE_ERR;
  __asm__("inb %%dx, %%al\n" : "=a" (c) : "d" (port));

  val16 = c;
  return val16;
}

#if defined(IDE_CD_DRIVER)
/* Envoyer une commande Packet */
u8 idepSendPacket(idePhysicalDiskID_t id, packet_t pckt, u8 *resp)
{
  u8 c;
  u16 port, val16;
  u32 i;
  u16 ta;

  idepInit(id);

  for(ta=0;ta<PACKET_TRY_AGAIN;ta++)
    {
      // ecrire tous les parametres
      port = ideAdapt[(id>>4)-1] + IDE_FEATURES;
      c = 0;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));
      port = port - IDE_FEATURES + IDE_COUNT;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));
      port = port - IDE_COUNT + IDE_SECT;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));
      port = port - IDE_SECT + IDE_CYLLOW;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));
      port = port - IDE_CYLLOW + IDE_CYLHIGH;
      c = 0x10;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));
      port = port - IDE_CYLHIGH + IDE_HEADDSK;
      c = 0xa0;
      c |= (id&0xf)<<4;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));
      
      port = port - IDE_HEADDSK + IDE_STAT2;
      L400NS(port);
      port = port - IDE_STAT2 + IDE_HEADDSK;

      // ecrire la commande
      port = port - IDE_HEADDSK + IDE_COMMAND;
      c = IDE_COMM_PACKET;
      __asm__("outb %%al, %%dx\n" : : "a" (c), "d" (port));

      port = port - IDE_COMMAND + IDE_STAT2;
      L400NS(port);
      port = port - IDE_STAT2 + IDE_COMMAND;

      // transférer le packet

      // attendre que le controleur ne soit plus occupe
      if((c=idepWait(id>>4, 0x80, 0x0))!=BLOCK_ERR_OK)
	return c;

      // attendre que le disque soit pret a transferer des donnees
      if((c=idepWait(id>>4, 0x8, 0x8))!=BLOCK_ERR_OK)
	return c;

      port = port - IDE_COMMAND + IDE_DATA;
      for(i=0;i<pckt.packetSize/2;i++)
	{
	  val16 = pckt.packetBuffer[i*2] | pckt.packetBuffer[i*2+1]<<8;
	  __asm__("outw %%ax, %%dx\n"
		  :
		  : "a" (val16), "d" (port));
	}
      
      // attendre que le controleur ne soit plus occupe
      if((c=idepWait(id>>4, 0x80, 0x0))!=BLOCK_ERR_OK)
	return c;

      port = port - IDE_DATA + IDE_COUNT;

      __asm__("inb %%dx, %%al\n" : "=a" (c) : "d" (port));
      port = port - IDE_COUNT + IDE_DATA;

      if((c & 7) == 2)
	{
	  // lire la réponse
	  for(i=0;i<pckt.responseSize/2;i++)
	    {
	      __asm__("inw %%dx, %%ax\n"
		      : "=a" (val16)
		      : "d" (port));
	      resp[i*2] = val16 & 0xff;
	      resp[i*2+1] = (val16>>8)&0xff;
	    }
	  return BLOCK_ERR_OK;
	}
      else if((c & 7) == 3 && !pckt.responseSize)
	return BLOCK_ERR_OK;
    }
  return BLOCK_ERR_NO_MEDIA;
}
#endif

/* Attendre que le controleur soit pret */
u8 idepWait(u8 adapt, u8 mask, u8 ok)
{
  u32 l;
  u8 c;
  u16 port = ideAdapt[adapt - 1] + IDE_STAT2;

  for(l=0;l<IDE_TIMEOUT;l++)
    {
      __asm__("inb %%dx, %%al\n" : "=a" (c) : "d" (port));
      if((c&mask) == ok)
	return BLOCK_ERR_OK;
    }

  return BLOCK_ERR_DISK;
}

s16 idepGetProtocol(idePhysicalDiskID_t id)
{
  u16 port, port2;
  u8 val, val2;

  idepInit(id);

  port = ideAdapt[(id>>4)-1] + IDE_SECT;
  port2 = ideAdapt[(id>>4)-1] + IDE_COUNT;
  __asm__("inb %%dx, %%al" : "=a" (val) : "d" (port));
  __asm__("inb %%dx, %%al" : "=a" (val2) : "d" (port2));

  if(val == 0x1 && val2 == 0x1)
    {
      port = port - IDE_SECT + IDE_CYLLOW;
      port2 = port2 - IDE_COUNT + IDE_CYLHIGH;
      __asm__("inb %%dx, %%al" : "=a" (val) : "d" (port));
      __asm__("inb %%dx, %%al" : "=a" (val2) : "d" (port2));

      if(val == 0 && val2 == 0)
	return ATA_PROTOCOL;
      else if(val == 0x14 && val2 == 0xeb)
	{
	  val = 0xa1;
	  port = port - IDE_CYLHIGH + IDE_COMMAND;

	  return ATAPI_PROTOCOL;
	}
      else
	return -BLOCK_ERR_DISK;
    }
  else
    return -BLOCK_ERR_DISK;
}
