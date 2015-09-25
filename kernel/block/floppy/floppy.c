/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Pilote disquette
*/
#ifdef FLOPPY_DRIVER
#include "floppy.h"

u32 floppyDMABuffer;
disksList_t floppyDisksList;
adapterType floppyAdapterType;
s32 mTick = 0;
bool motor = FALSE;
bool dChange = FALSE;
u8 fdcTrack = 0xff;
u8 sr0 = 0;
floppyDrvGeom_t geometry = { DG144_HEADS, DG144_TRACKS, DG144_SPT };
u8 status[7] = { 0 };
u8 statsz = 0;
bool done = FALSE;
/* volatile s32 tmout = 0; */

u8 floppyInitDriver(void)
{
  u8 i;

  floppyDisksList.nDisks = 1;
  floppyDisksList.disks = (disksListEntry_t*)kMalloc(sizeof(disksListEntry_t));
  floppyDisksList.disks->disk = 0;
  floppyDisksList.disks->next = NULL;

  /* Allouer un tampon DMA */
  floppyDMABuffer = kMallocForDMA(512);

  /* Ajouter l'IRQ 6 */
  kAddIrq(floppyLowIRQ6, 6);

  /* Reset */
  floppyLowReset();

  /* version du controleur */
  floppyLowSendByte(CMD_VERSION);
  i = floppyLowGetByte();
  
  if (i == 0x80)
    floppyAdapterType = nec765; /* NEC765 */
  else
    floppyAdapterType = extAdapter; /* controlleur etendu */

  return BLOCK_ERR_OK;
}

u8 floppyCleanupDriver(void)
{
  /* Normalement, il faudrait restaurer l'ancienne IRQ6 mais ce n'est pas important car si le pilote est déchargé, c'est que le système va être éteint */

#ifdef TESTDOS
  set_irq_handler(6, NULL, &irqsave);
#endif

  /* pas la peine de forcer le moteur */
  outb(FDC_DOR,0x0c);

  return BLOCK_ERR_OK;
}

u8 floppyGetInf(diskID_t id, disk_t *disk)
{
  if(id != 0) /* Dans cette version, 1 seul lecteur supporté */
    return BLOCK_ERR_DISK;

  disk->id = id;
  disk->nSects = 0; /* amovible */
  disk->type = floppy;
  disk->model[0] = 0;
  disk->serial[0] = 0;
  disk->spec.floppy.type = floppyAdapterType;

  /* Fonctions supportées */
  disk->features.init = TRUE;
  disk->features.read = TRUE;
  disk->features.write = TRUE;
  disk->features.getInf = TRUE;
  disk->features.getSize = TRUE;
  disk->features.lock = FALSE;
  disk->features.unlock = FALSE;
  disk->features.isChanged = TRUE;

  return BLOCK_ERR_OK;
}

u8 floppyInit(diskID_t id)
{
  if(id != 0) /* Dans cette version, 1 seul lecteur supporté */
    return BLOCK_ERR_DISK;

  floppyLowReset();
}

u8 floppyRead(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  u32 c;

  for(c=0;c<cnt;c++)
    {
      if(floppyLowReadBlock(log+c, &(buffer[c*512]))==FALSE)
	return BLOCK_ERR_UNKNOWN_ERROR;
    }

  return BLOCK_ERR_OK;
}

u8 floppyWrite(diskID_t id, u32 cnt, lbaAddress_t log, u8 *buffer)
{
  u32 c;

  for(c=0;c<cnt;c++)
    {
      if(floppyLowWriteBlock(log+c, &(buffer[c*512]))==FALSE)
	return BLOCK_ERR_UNKNOWN_ERROR;
    }

  return BLOCK_ERR_OK;
}

u8 floppyGetSize(diskID_t id, u32 *size)
{
  floppyDrvGeom_t geom;

  if(floppyLowLogDisk(&geom)==FALSE)
    return BLOCK_ERR_NO_MEDIA;

  /* 1.68 Mo = 3360 secteurs */
  if(geom.heads == DG168_HEADS && geom.tracks == DG168_TRACKS && geom.spt == DG168_SPT)
    (*size) = 3360;

  /* 1.44 Mo = 2880 secteurs */
  else if(geom.heads == DG144_HEADS && geom.tracks == DG144_TRACKS && geom.spt == DG144_SPT)
    (*size) = 2880;

  else
    return BLOCK_ERR_UNIMPLEMENTED;

  return BLOCK_ERR_OK;
}

u8 floppyLock(diskID_t id)
{
  return BLOCK_ERR_UNIMPLEMENTED;
}

u8 floppyUnlock(diskID_t id)
{
  return BLOCK_ERR_UNIMPLEMENTED;
}

s8 floppyIsChanged(diskID_t id)
{
  if(id != 0)
    return -(BLOCK_ERR_DISK);

  if (inb(FDC_DIR) & 0x80) {
    dChange = TRUE;
    return 1;
  }
  return 0;
}

void floppyLowReset(void)
{
  /* stop moteur et desactive IRQ/DMA */
   outb(FDC_DOR,0);
   
   mTick = 0;
   motor = FALSE;

   /* (500K/s) */
   outb(FDC_DRS,0);

   /* interruptions redispo */
   outb(FDC_DOR,0x0c);

   
   done = TRUE;
   floppyLowWaitFDC(TRUE);

   /* specifie les temps du lecteur */
   floppyLowSendByte(CMD_SPECIFY);
   floppyLowSendByte(0xdf);  /* SRT = 3ms, HUT = 240ms */
   floppyLowSendByte(0x02);  /* HLT = 16ms, ND = 0 */
   
   /* changement de disk propre */
   floppyLowSeek(1);
   floppyLowRecalibrate();

   dChange = FALSE;
}

void floppyLowMotorOn(void)
{
   if (!motor) {
      mTick = -1;     
      outb(FDC_DOR,0x1c);
      kSleep(500); 
      motor = TRUE;
   }
}

void floppyLowMotorOff(void)
{
   if (motor) {
      mTick = 36;   
   }
}

void floppyLowRecalibrate(void)
{
   floppyLowMotorOn();
   floppyLowSendByte(CMD_RECAL);
   floppyLowSendByte(0);
   floppyLowWaitFDC(TRUE);
   floppyLowMotorOff();
}

bool floppyLowSeek(u8 track)
{
   if (fdcTrack == track) 
     return TRUE;
   
   floppyLowMotorOn();
   
   floppyLowSendByte(CMD_SEEK);
   floppyLowSendByte(0);
   floppyLowSendByte(track);

   if (!floppyLowWaitFDC(TRUE))
     return FALSE;     /* time out */

   kSleep(15);
   
   floppyLowMotorOff();
   
   if ((sr0 != 0x20) || (fdcTrack != track))
     return FALSE;
   else
     return TRUE;
}

bool floppyLowLogDisk(floppyDrvGeom_t *g)
{
   floppyLowReset();

   geometry.heads = DG168_HEADS;
   geometry.tracks = DG168_TRACKS;
   geometry.spt = DG168_SPT;

   if (floppyLowReadBlock(20,NULL)) {
      /* 1.68M */
      if (g) {
	 g->heads = geometry.heads;
	 g->tracks = geometry.tracks;
	 g->spt = geometry.spt;
      }
      return TRUE;             
   }
   
   /* sinon */
   geometry.heads = DG144_HEADS;
   geometry.tracks = DG144_TRACKS;
   geometry.spt = DG144_SPT;

   if (floppyLowReadBlock(17,NULL)) {
      /* 1.44M */
      if (g) {
	 g->heads = geometry.heads;
	 g->tracks = geometry.tracks;
	 g->spt = geometry.spt;
      }
      return TRUE;
   }
   
   /* sinon non supporte */
   return FALSE;
}

bool floppyLowReadBlock(u32 block,u8 *blockBuff)
{
  return floppyLowRW(block,blockBuff,TRUE);
}

bool floppyLowWriteBlock(u32 block,u8 *blockBuff)
{
  return floppyLowRW(block,blockBuff,FALSE);
}

bool floppyLowRW(u32 block, u8 *blockBuff, bool read)
{
   u8 head,track,sector,tries,i;

   /* adresse logique a physique */
   floppyLowConvertBlockToHTS(block,&head,&track,&sector);
   //   printf("block %d = %d:%02d:%02d\n",block,head,track,sector);
   
   floppyLowMotorOn();

   if (!read && blockBuff) {
     #ifdef TESTDOS
     movedata(_my_ds(), (long)blockBuff, _dos_ds, floppyDMABuffer, 512);
     #else
     kMemCpy((long)blockBuff,floppyDMABuffer,512);
     #endif
   }
   
   for (tries = 0;tries < 3;tries++) {
      /* check for diskchange */
      if (inb(FDC_DIR) & 0x80) {
	 dChange = TRUE;
	 floppyLowSeek(1);  
	 floppyLowRecalibrate();
	 floppyLowMotorOff();
	 return FALSE;
      }

      /* tete sur la piste a droite */
      if (!floppyLowSeek(track)) {
	 floppyLowMotorOff();
	 return FALSE;
      }
      
      outb(FDC_CCR,0);
      
      if (read) {
	 dmaXfer(2,floppyDMABuffer,512,FALSE);
	 floppyLowSendByte(CMD_READ);
      } else {
	 dmaXfer(2,floppyDMABuffer,512,TRUE);
	 floppyLowSendByte(CMD_WRITE);
      }
      
      floppyLowSendByte(head << 2);
      floppyLowSendByte(track);
      floppyLowSendByte(head);
      floppyLowSendByte(sector);
      floppyLowSendByte(2);               
      floppyLowSendByte(geometry.spt);
      if (geometry.spt == DG144_SPT)
	floppyLowSendByte(DG144_GAP3RW);  
      else
	floppyLowSendByte(DG168_GAP3RW);  
      floppyLowSendByte(0xff);            
      
     if (!floppyLowWaitFDC(FALSE))
	return FALSE;   
      
      if ((status[0] & 0xc0) == 0) break;   
   
      floppyLowRecalibrate();  
   }
   
   /* stop moteur */
   floppyLowMotorOff();

   if (read && blockBuff) {
     #ifdef TESTDOS
     movedata(_dos_ds, floppyDMABuffer, _my_ds(), (long)blockBuff, 512);
     #else
     kMemCpy(floppyDMABuffer,(long)blockBuff,512);
     #endif
   }

   /*   printf("Informations : ");
   for (i = 0;i < statsz;i++)
     printf("%02x ",status[i]);

   printf("\n");
   */

   return (tries != 3);
}

/* floppyLowSendByte, sendbyte dans le pilote d'origine, tirée du manuel d'Intel */
void floppyLowSendByte(u8 byte)
{
   volatile int msr;
   int tmo;
   
   for (tmo = 0;tmo < 128;tmo++) {
      msr = inb(FDC_MSR);
      if ((msr & 0xc0) == 0x80) {
	 outb(FDC_DATA,byte);
	 return;
      }
      inb(0x80);   /* delai */
   }
}

/* floppyLowGetByte, getbyte dans le pilote d'origine, tirée du manuel d'Intel */
s8 floppyLowGetByte(void)
{
   volatile int msr;
   int tmo;
   
   for (tmo = 0;tmo < 128;tmo++) {
      msr = inb(FDC_MSR);
      if ((msr & 0xd0) == 0xd0) {
	 return inb(FDC_DATA);
      }
      inb(0x80);   
   }

   return -1;   
}

bool floppyLowWaitFDC(bool sensei)
{
  /* tmout = 18;   
     
     (commentaire: attend signalisation fin de commande IRQ6)
   while (!done && tmout)
     ;
  */

  /* Provisoire : boucle de tempo pour remplacer le timeout */
  {
    u32 timeOut;
    for(timeOut = 0;timeOut < FLOPPY_TIMEOUT && !done;timeOut++)
      ;
  }

   /* resultat */
   statsz = 0;
   while ((statsz < 7) && (inb(FDC_MSR) & (1<<4))) {
      status[statsz++] = floppyLowGetByte();
   }

   if (sensei) {
      
      floppyLowSendByte(CMD_SENSEI);
      sr0 = floppyLowGetByte();
      fdcTrack = floppyLowGetByte();
   }
   
   done = FALSE;
   
   /*
   if (!tmout) {
      (commentaire: timed out! )
      if (inb(FDC_DIR) & 0x80)  (commentaire: cf changement de disk )
	dchange = TRUE;
      
      return FALSE;
   } else*/
     return TRUE;
}

void floppyLowIRQ6(void)
{
   /* operation finie ou pas */
   done = TRUE;

   outb(0x20,0x20);
}

void floppyLowConvertBlockToHTS(u32 block, u8 *head, u8 *track, u8 *sector)
{
   *head = (block % (geometry.spt * geometry.heads)) / (geometry.spt);
   *track = block / (geometry.spt * geometry.heads);
   *sector = block % geometry.spt + 1;
}
#endif
