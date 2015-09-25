#include <drivers/fs/fat/fat.h>
#include <drivers/block/block/block.h>
//#include <vfs/vfs.h>
extern u8* known_fs[256];
/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : FAT_search_next
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : entier 32 bits contenant la valeur du premier emplacement
** 		de file descriptor dispo.
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.01
------------------------------------------------------------------------*/

u32 FAT_search_next()
{
    int i;
    
    for(i=0;i<FAT_nb_files_open;i++)
    {
	if(!FAT_open_files_list[i]->used)return i;
    }
    
    return FAT_nb_files_open;
}


unsigned long FAT_FirstSectorofCluster(unsigned long N)
{
  return ((N-2) * FAT_vars[FAT_cur_vol]->bpb->BPB_SecPerClus) + FAT_vars[FAT_cur_vol]->FirstDataSector;
}

unsigned short FAT_Sect(unsigned long sec)
{
  return (1+ sec % FAT_vars[FAT_cur_vol]->sectors_per_track);
}

unsigned short FAT_Cylindre(unsigned long sec)
{
  return (sec / (FAT_vars[FAT_cur_vol]->NumHeads*FAT_vars[FAT_cur_vol]->sectors_per_track));
}

unsigned short FAT_Head(unsigned long sec)
{
  return ((sec / FAT_vars[FAT_cur_vol]->sectors_per_track) % FAT_vars[FAT_cur_vol]->NumHeads);
}


/* Normalement, c'est la seule fonction qui doit dpendre du type de FAT */
unsigned short FAT_NextCluster12(unsigned short clus)
{
 	unsigned char fat[512], fat2[512];
 unsigned short FatEntOffset, FatSecNum, FatOffset;
 unsigned short FatValue;

 FatOffset = clus + (clus / 2);
 FatSecNum = FAT_vars[FAT_cur_vol]->BaseLBA + FAT_vars[FAT_cur_vol]->bpb->BPB_ResvdSecCnt + (FatOffset / FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec);
 FatEntOffset = FatOffset % FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec;

 //ReadSector(Id, &fat, Sect(FatSecNum), FAT_Cylindre(FatSecNum), FAT_Head(FatSecNum));
 ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FatSecNum, fat);

 if (FatEntOffset == (FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec)) {
   //   ReadSector(Id, &fat2, Sect(FatSecNum+1), FAT_Cylindre(FatSecNum+1),
   //			FAT_Head(FatSecNum+1));
   ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FatSecNum+1, fat2);
   FatValue = (fat2[0] << 8) + fat[FatEntOffset];
 } else FatValue = (fat[FatEntOffset+1] << 8) + fat[FatEntOffset];

 if ((clus & 0x0001) == 1) return (FatValue >> 4);
 else return (FatValue & 0x0FFF);
}


unsigned short FAT_NextCluster16(unsigned short clus)
{  unsigned char fat[512];
 unsigned short FatEntOffset, FatSecNum, FatOffset;
 unsigned short FatValue;

 FatOffset = clus*2;
 FatSecNum = FAT_vars[FAT_cur_vol]->BaseLBA + FAT_vars[FAT_cur_vol]->bpb->BPB_ResvdSecCnt + (FatOffset / FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec);
 FatEntOffset = FatOffset % FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec;

 //ReadSector(Id, &fat, Sect(FatSecNum), FAT_Cylindre(FatSecNum), FAT_Head(FatSecNum));
 ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FatSecNum, fat);

 FatValue = (unsigned short)fat[FatEntOffset];

 return FatValue;
}

unsigned long FAT_NextCluster32(unsigned long clus)
{  unsigned char fat[512];
 unsigned short FatEntOffset, FatSecNum, FatOffset;
 unsigned short FatValue;

 FatOffset = clus*4;
 FatSecNum = FAT_vars[FAT_cur_vol]->BaseLBA + FAT_vars[FAT_cur_vol]->bpb->BPB_ResvdSecCnt + (FatOffset / FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec);
 FatEntOffset = FatOffset % FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec;

 //ReadSector(Id, &fat, Sect(FatSecNum), FAT_Cylindre(FatSecNum), FAT_Head(FatSecNum));
 ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FatSecNum, fat);

 FatValue = (unsigned long)fat[FatEntOffset];

 return (FatValue & 0x0FFFFFFF);

}

unsigned long NextCluster(unsigned long clus)
{
  if (FAT_vars[FAT_cur_vol]->FATtype == 12) return (unsigned long)FAT_NextCluster12(clus);
  if (FAT_vars[FAT_cur_vol]->FATtype == 16) return (unsigned long)FAT_NextCluster16(clus);
  return (unsigned long)(FAT_NextCluster32(clus));
}


int match(unsigned char *s1, unsigned char length, unsigned char *s2)
{ int i, j;
 s1[length] = 0;
 i=j=0;
 while ((s1[i] != 0) && (s2[j] != 0)) {

   /* Les noms sont case INsensitive */
   if ((s1[i] <= 122) && (s1[i] >= 97)) s1[i]-=32;
   if ((s2[j] <= 122) && (s2[j] >= 97)) s2[j]-=32;
   /*	if (s2[j] == '.') {
	j++;
	i = length-3;
	} */
   if (s2[j] == '*') {
     /*		if (s2[j+1] == '.') {
		j+=2;
		i = length-3;
		}*/
     while ((s1[i] != 0) && (s1[i] != s2[j+1])) i++;
     if (s1[i] != s2[j+1]) return 0;
     j++;
   } // if *
   else if (s2[j] != '?') {
     if (s1[i] != s2[j]) return 0;
   } // if != ?
   if ((s1[i] != 0) && (s2[j] != 0)) {
     i++;
     j++;
   }

   while ((s1[i] == ' ') || ((unsigned int)s1[i] == 255)) {
     i++;
   }


 }  // while
 if ((s1[i] == 0) && (s2[j] == 0)) return 1; else return 0;
}


int FAT_FindNext(LongDir *dir2)
{ DirEntry *dir;
 LnDirEntry *lndir;
 unsigned char longname[19][25], Ni, Ni2, fname[255], ext[3];
 unsigned short j, length;
 short k, l, m;
 unsigned char *Sector2;
 unsigned long FSec;

 Sector2 = (unsigned char *)malloc(FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec);

 if (FAT_vars[FAT_cur_vol]->ClusNum != 0)
   FSec = FAT_FirstSectorofCluster(FAT_vars[FAT_cur_vol]->ClusNum);
 else FSec = FAT_vars[FAT_cur_vol]->FirstRootDirSecNum;

 dir=(DirEntry *)&Sector2[FAT_vars[FAT_cur_vol]->CurPos*32];
 //if (LastSecNum != FSec+SecNum) {
 /*ReadSector(Id, Sector2, Sect(FSec+SecNum),
  *			FAT_Cylindre(FSec+SecNum),
  *			    FAT_Head(FSec+SecNum));
  */
 ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FSec+FAT_vars[FAT_cur_vol]->SecNum, Sector2);
 FAT_vars[FAT_cur_vol]->LastSecNum = FSec+FAT_vars[FAT_cur_vol]->SecNum;
 //}


 Ni = 0;
 while ((dir->Name[0] != 0)) {
   //        printf("%d, %d, %d\n", dir->Attr, Attr, dir->Attr & Attr);
   if ((dir->Name[0] != 0xE5) && (((dir->Attr & FAT_vars[FAT_cur_vol]->Attr) != 0)
				  || ((dir->Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME))) {
     if ((dir->Attr & ATTR_LONG_NAME) == ATTR_LONG_NAME)
       {
	 for (j=0; j < 25; j++) longname[Ni][j] = 0;
	 Ni2 = 0;
	 lndir = (LnDirEntry *)&Sector2[FAT_vars[FAT_cur_vol]->CurPos*32];
	 for (j=1; j<10; j++)
	   { 
	     if (lndir->lname1[j] != 0) {
	       longname[Ni][Ni2] = lndir->lname1[j];
	       Ni2++;
	     }
	   }
	 for (j=0; j<12; j++) {
                     
	   if (lndir->lname2[j] != 0) {
	     longname[Ni][Ni2] = lndir->lname2[j];
	     Ni2++;
	   }
	 }
	 for (j=0; j<4; j++) {
                     
	   if (lndir->lname3[j] != 0) {
	     longname[Ni][Ni2] = lndir->lname3[j];
	     Ni2++;
	   }
	 }
	 Ni++;
       } else {
	 if (Ni != 0) /* Le nom est un nom long */
	   {
                        
	     m = 0;
	     for (k=Ni-1; k >= 0; k--)
	       for (l=0; (l < 25) & (longname[k][l] != 0); l++)
		 {
		   fname[m] = longname[k][l];
		   m++;
		 }
	     length = m;
	   } else {
	     m = 0;
	     for (j=0; j < 8; j++)
	       if (dir->Name[j] != ' ')
		 {
		   fname[m] = dir->Name[j];
		   m++;
		 }
	     ext[0] = 0;
	     for (j=8; j < 11; j++)
	       /*if (dir->Name[j] != ' ')*/ ext[j-8] = dir->Name[j];
	     if ((ext[0] != 0) && (ext[0] != 32)) {
	       fname[m] = '.';
	       m++;
	       fname[m] = ext[0];
	       m++;
	       fname[m] = ext[1];
	       m++;
	       fname[m] = ext[2];
	       m++;
	     }
	     fname[m] = 0;
	     length = m;
	   }

                
	 if (match(fname, length, FAT_vars[FAT_cur_vol]->name) == 1) {
	   FAT_vars[FAT_cur_vol]->CurPos++;

	   if (FAT_vars[FAT_cur_vol]->CurPos >= (FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec / 32)) {
	     FAT_vars[FAT_cur_vol]->LastSecNum = FAT_vars[FAT_cur_vol]->SecNum+FSec;
	     FAT_vars[FAT_cur_vol]->SecNum++;
	     if ((FAT_vars[FAT_cur_vol]->SecNum >= FAT_vars[FAT_cur_vol]->bpb->BPB_SecPerClus)
		 && (FSec+FAT_vars[FAT_cur_vol]->SecNum > FAT_vars[FAT_cur_vol]->FirstRootDirSecNum+FAT_vars[FAT_cur_vol]->RootDirSectors)) {
	       FAT_vars[FAT_cur_vol]->SecNum = 0;
	       FAT_vars[FAT_cur_vol]->ClusNum = NextCluster(FAT_vars[FAT_cur_vol]->ClusNum);
	       FSec = FAT_FirstSectorofCluster(FAT_vars[FAT_cur_vol]->ClusNum);
	     }
	     //				ReadSector(Id, Sector2, Sect(FSec+SecNum),
	     //  					FAT_Cylindre(FSec+SecNum), FAT_Head(FSec+SecNum));
	     //                                ideRead(Id, 1, FSec+SecNum, Sector2);
	     FAT_vars[FAT_cur_vol]->CurPos = 0;
	   }

	   j = 0;
	   while (fname[j] != 0) {
	     dir2->Name[j] = fname[j];
	     j++;
	   }
	   dir2->Name[j] = fname[j];
	   dir2->Attr = dir->Attr;
	   dir2->NTRes = dir->NTRes;
	   dir2->CrtTimeTenth = dir->CrtTimeTenth;
	   dir2->CrtTime = dir->CrtTime;
	   dir2->CrtDate = dir->CrtDate;
	   dir2->LstAccDate = dir->LstAccDate;
	   dir2->FstClusHi = dir->FstClusHi;
	   dir2->WrtTime = dir->WrtTime;
	   dir2->WrtDate = dir->WrtDate;
	   dir2->FstClusLo = dir->FstClusLo;
	   dir2->FileSize = dir->FileSize;

	   free(Sector2);
	   return 0;
	 }

	 Ni = 0;
       }

   }


   FAT_vars[FAT_cur_vol]->CurPos++;
   if (FAT_vars[FAT_cur_vol]->CurPos >= (FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec / 32)) {
     FAT_vars[FAT_cur_vol]->LastSecNum = FAT_vars[FAT_cur_vol]->SecNum+FSec;
     FAT_vars[FAT_cur_vol]->SecNum++;
     if ((FAT_vars[FAT_cur_vol]->SecNum >= FAT_vars[FAT_cur_vol]->bpb->BPB_SecPerClus)
	 && (FSec+FAT_vars[FAT_cur_vol]->SecNum > FAT_vars[FAT_cur_vol]->FirstRootDirSecNum+FAT_vars[FAT_cur_vol]->RootDirSectors)) {
       FAT_vars[FAT_cur_vol]->SecNum = 0;
       FAT_vars[FAT_cur_vol]->ClusNum = NextCluster(FAT_vars[FAT_cur_vol]->ClusNum);
       FSec = FAT_FirstSectorofCluster(FAT_vars[FAT_cur_vol]->ClusNum);
     }
     //                ReadSector(Id, Sector2, Sect(FSec+SecNum),
     //                      FAT_Cylindre(FSec+SecNum), FAT_Head(FSec+SecNum));
     ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FSec+FAT_vars[FAT_cur_vol]->SecNum, Sector2);
     FAT_vars[FAT_cur_vol]->CurPos = 0;
   }
   dir = (DirEntry *)&Sector2[FAT_vars[FAT_cur_vol]->CurPos*32];

 } // while
 free(Sector2);
 return 1;
}

int FAT_FindFirst(unsigned char *n, unsigned char A, LongDir *dir2)
{
  FAT_vars[FAT_cur_vol]->CurPos = 0;
  FAT_vars[FAT_cur_vol]->name = n;
  FAT_vars[FAT_cur_vol]->Attr = A;
  FAT_vars[FAT_cur_vol]->SecNum = 0;
  FAT_vars[FAT_cur_vol]->ClusNum = FAT_vars[FAT_cur_vol]->FClus;
  return FAT_FindNext(dir2);
}

u32 FAT_open(u8* fname, u16 flag,u16 mode)
{  

    LongDir *dir;
    unsigned long fclus;
    u32 fd;

    dir = (LongDir *)malloc(sizeof(LongDir));

    if (FAT_FindFirst(fname, ATTR_ARCHIVE || ATTR_DIRECTORY, dir) == 0)
    {
	fd = FAT_search_next();		// recherche le prochain file descriptor dispo
	if(fd == FAT_size_of_files_list)
	{
	    realloc(open_files_list, sizeof(file_descriptor*)*(fd+1));
	    FAT_open_files_list[fd] = (FAT_file*) malloc(sizeof(FAT_file));
	    size_of_files_list++;
        }
	FAT_open_files_list[fd]->used = 1;	// le declare occupe
	FAT_nb_files_open++;		// repercute le nb de fichiers ouverts
    
        fclus = (dir->FstClusHi << 16) + (dir->FstClusLo);
     
	FAT_open_files_list[fd]->fclus = fclus;
        FAT_open_files_list[fd]->pos = 0;
        FAT_open_files_list[fd]->Open = 1;
        FAT_open_files_list[fd]->size = dir->FileSize;
        FAT_open_files_list[fd]->clus = fclus;
        FAT_open_files_list[fd]->Snum = 0;
        FAT_open_files_list[fd]->Soff = 0;

        free(dir);

        return fd;

    } 
    else
    {
        free(dir);
        return -1;
    }
}

u8  FAT_close(u32 fd)
{
    if(!FAT_open_files_list[fd]->used)return -1; // faudrait voir a fermer un fichier qui est bien ouvert ...
    FAT_open_files_list[fd]->used = 0;		// c'est libre 
    FAT_nb_files_open--;
    return 0;    
}

u32 FAT_ftell(u32 fd)
{
    return FAT_open_files_list[fd]->pos;
}


u16 FAT_fseek(u32 fd, u32 offset)
{
    unsigned long /*clus, pos,*/ BytesPerClus;

    if (offset > FAT_open_files_list[fd]->size) return F_READ_ERROR;
    if (offset == FAT_open_files_list[fd]->pos) return 0;
    if (offset < FAT_open_files_list[fd]->pos) 
    {
	FAT_open_files_list[fd]->pos = 0;
	FAT_open_files_list[fd]->Snum = 0;
	FAT_open_files_list[fd]->clus = FAT_open_files_list[fd]->fclus;
    }

    BytesPerClus = (FAT_vars[fd]->bpb->BPB_SecPerClus*FAT_vars[fd]->bpb->BPB_BytesPerSec);

    while ((offset / BytesPerClus) > (FAT_open_files_list[fd]->pos / BytesPerClus)) 
    {
	FAT_open_files_list[fd]->clus = NextCluster(FAT_open_files_list[fd]->clus);
	FAT_open_files_list[fd]->pos += BytesPerClus;
    }

    FAT_open_files_list[fd]->pos = offset;
    FAT_open_files_list[fd]->Snum+= (offset / FAT_vars[fd]->bpb->BPB_BytesPerSec)-(FAT_open_files_list[fd]->pos / FAT_vars[fd]->bpb->BPB_BytesPerSec);

    return 0;
}

u8 FAT_feof(u32 fd)
{
  return (FAT_open_files_list[fd]->pos >= FAT_open_files_list[fd]->size);
}


u32 FAT_read(u32 fd, void *buf, u32 size)
{
    /*
    
unsigned short FAT_fread(file *f, void *buf, unsigned long num, unsigned long *read)

{*/ 
    unsigned long clus, i,  pos, j, read;
    unsigned short  Snum;
    unsigned char *buffer;
    unsigned long FSec;


    if (FAT_open_files_list[fd]->Open == 0) return F_NOT_OPEN;
    if (FAT_open_files_list[fd]->pos >= FAT_open_files_list[fd]->size) return F_READ_ERROR;

    buffer = (unsigned char *)malloc(FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec);

    Snum = FAT_open_files_list[fd]->Snum;
    clus = FAT_open_files_list[fd]->clus;
    pos = FAT_open_files_list[fd]->Soff;

    read = 0;
    i = 0;
    FSec = FAT_FirstSectorofCluster(clus);

    while ((i < size) && (FAT_open_files_list[fd]->pos < FAT_open_files_list[fd]->size)) 
    {

	if (Snum >= FAT_vars[FAT_cur_vol]->bpb->BPB_SecPerClus) 
	{
    	    clus = NextCluster(clus);
    	    if (clus == FAT_vars[FAT_cur_vol]->EOC)
    	    {
		free(buffer);
		return 0;
    	    }
    	    Snum = 0;
    	    FSec = FAT_FirstSectorofCluster(clus);
	}

 
	ideRead(FAT_vars[FAT_cur_vol]->Id, 1, FSec+Snum, (unsigned char *)buffer);

	for (j=pos; (j < 512) && (i < size) && (FAT_open_files_list[fd]->pos < FAT_open_files_list[fd]->size); j++) 
	{
    	    ((unsigned char *)buf)[i] = buffer[j];
    	    i++;
    	    FAT_open_files_list[fd]->pos++;
	}

	if (j == 512) Snum++;
	pos = 0;
	read += (j-pos);

    }

    FAT_open_files_list[fd]->Soff = (FAT_open_files_list[fd]->Soff + size) % FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec;
    FAT_open_files_list[fd]->Snum = Snum;
    FAT_open_files_list[fd]->clus = clus;
    free(buffer);
    return 0;

}


u16 FAT_chdir(u8 *newdir)
{
    LongDir dirinfo;
    if (FAT_FindFirst(newdir, ATTR_DIRECTORY, &dirinfo) == 1) return 3;
    FAT_vars[FAT_cur_vol]->ClusNum = (dirinfo.FstClusHi << 16)+dirinfo.FstClusLo;
    FAT_vars[FAT_cur_vol]->FClus = FAT_vars[FAT_cur_vol]->ClusNum;
    FAT_vars[FAT_cur_vol]->SecNum = 0;
 //FSecNum = FirstSectorofCluster((dirinfo.FstClusHi << 16)+dirinfo.FstClusLo);
    return 0;
}

u8 FAT_change_vol(u8 new_vol)
{
    if(new_vol < FAT_nb_vols)
    {
	FAT_cur_vol = new_vol;
	return FAT_cur_vol;
    }
    else
    {
	return -1;
    }
}

u32 FAT_write(u32 fd, void *buf, u32 size)
{
    return 0;
}

u16 FAT_scandir(u8 * dir, struct dirent ***namelist, u16 (*select)(const struct dirent *), u16 (*compar)(const struct dirent **, const struct dirent **))
{
    return 0;
}


u16 FAT_stat(u8*fname, struct stat *buf)
{
    return 0;
}

u16 FAT_fstat(u32 filedes, struct stat *buf)
{
    return 0;
}

u16 FAT_quit()
{
    return 0;
}

u8 FAT_init(u16 device, DRIVER* drv)//, unsigned long LBA)
{
    u32 LBA;
  FAT32 *fat32;
  FAT12_16 *fat;
  unsigned long FATSz, TotSec, DataSec, CountofClusters;
 /* unsigned char i;*/
  unsigned char readres;
    disk_t *part = (disk_t*) malloc(sizeof(disk_t));
    blockGetInf(device,part);
    switch(part->type)
    {
#ifdef FLOPPY_DRIVER
	case floppy:
	    LBA = 0;
	    break;
#endif
#ifdef IDE_HD_DRIVER
	case ideHd:
	    LBA = part->spec.ideHd.lba;
	    break;
#endif
#ifdef IDE_CD_DRIVER
	case ideCd:
	    LBA = 0;
	    break;
#endif
	default:
	    LBA = 0;
	    break;
    }
    drv->vol_descriptor = FAT_nb_vols;
    realloc(FAT_vars,(FAT_nb_vols+1)*sizeof(FAT_DRIVER_VARS**));
    FAT_vars[FAT_nb_vols] = (FAT_DRIVER_VARS*) malloc(sizeof(FAT_DRIVER_VARS));
    FAT_change_vol(FAT_nb_vols);
    FAT_nb_vols++;

    FAT_vars[FAT_cur_vol]->BaseLBA = LBA;

    // ??? a virer ? en principe c'est pas lui qui s'en occupe !
  ideInitDriver();

  FAT_vars[FAT_cur_vol]->Id = device;
  FAT_vars[FAT_cur_vol]->bpb=(BPB *)&FAT_vars[FAT_cur_vol]->Sector;
  //if (ReadSector(Id, &Sector, 1, 0, 1) != 0) return 0;
  if ((readres = ideRead(device, 1, LBA, FAT_vars[FAT_cur_vol]->Sector)) != 0)
    {
      //printf("%s%d\n", "ideRead a renvoy: ", readres);
      return 0;
    }

  FAT_vars[FAT_cur_vol]->sectors_per_track = FAT_vars[FAT_cur_vol]->bpb->BPB_SecPerTrk;
  FAT_vars[FAT_cur_vol]->NumHeads = FAT_vars[FAT_cur_vol]->bpb->BPB_NumHeads;
  //Tracks = (bpb->BPB_TotSec16 + bpb->BPB_TotSec32) / sectors_per_track;
  //cylinders = Tracks / NumHeads;

  if (FAT_vars[FAT_cur_vol]->bpb->BPB_FATSz16 == 0)  /* Probablement une FAT32 */
    fat32 = (FAT32 *)&FAT_vars[FAT_cur_vol]->Sector[36];
  else fat = (FAT12_16 *)&FAT_vars[FAT_cur_vol]->Sector[36];

  FAT_vars[FAT_cur_vol]->RootDirSectors = ((FAT_vars[FAT_cur_vol]->bpb->BPB_RootEntCnt * 32) + (FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec -1)) / FAT_vars[FAT_cur_vol]->bpb->BPB_BytesPerSec;

  /* C'est ce calcul qui pose problme, puisqu'il sous-entend que si BPB_FATSz16
   * est nul, alors c'est une FAT32 (BPB_FATSz32 n'existe que si c'est une FAT32)
   * donc on se demande pourquoi M$ nous fais faire tous ces calculs! */
  if (FAT_vars[FAT_cur_vol]->bpb->BPB_FATSz16 != 0) FATSz = FAT_vars[FAT_cur_vol]->bpb->BPB_FATSz16;
  else FATSz = fat32->BPB_FATSz32;

  if (FAT_vars[FAT_cur_vol]->bpb->BPB_TotSec16 != 0) TotSec = FAT_vars[FAT_cur_vol]->bpb->BPB_TotSec16;
  else TotSec = FAT_vars[FAT_cur_vol]->bpb->BPB_TotSec32;

  DataSec = TotSec - (FAT_vars[FAT_cur_vol]->bpb->BPB_ResvdSecCnt + (FAT_vars[FAT_cur_vol]->bpb->BPB_NumFATs * FATSz) + FAT_vars[FAT_cur_vol]->RootDirSectors);

  CountofClusters = DataSec / FAT_vars[FAT_cur_vol]->bpb->BPB_SecPerClus;

  if (CountofClusters < 4085) FAT_vars[FAT_cur_vol]->FATtype = 12;
  else if (CountofClusters < 65525) FAT_vars[FAT_cur_vol]->FATtype = 16;
  else FAT_vars[FAT_cur_vol]->FATtype = 32;

  if ((FAT_vars[FAT_cur_vol]->bpb->BPB_FATSz16 == 0) && (FAT_vars[FAT_cur_vol]->FATtype != 32)) return 0;

  FAT_vars[FAT_cur_vol]->FirstDataSector = FAT_vars[FAT_cur_vol]->bpb->BPB_ResvdSecCnt + (FAT_vars[FAT_cur_vol]->bpb->BPB_NumFATs * FATSz) + FAT_vars[FAT_cur_vol]->RootDirSectors + LBA;

  if (FAT_vars[FAT_cur_vol]->FATtype == 12) FAT_vars[FAT_cur_vol]->EOC = EOC12;
  else if (FAT_vars[FAT_cur_vol]->FATtype == 16) FAT_vars[FAT_cur_vol]->EOC = EOC16;
  else if (FAT_vars[FAT_cur_vol]->FATtype == 32) FAT_vars[FAT_cur_vol]->EOC = EOC32;

  if (FAT_vars[FAT_cur_vol]->FATtype == 32) FAT_vars[FAT_cur_vol]->FirstRootDirSecNum = FAT_FirstSectorofCluster(fat32->BPB_RootClus);
  else FAT_vars[FAT_cur_vol]->FirstRootDirSecNum = FAT_vars[FAT_cur_vol]->bpb->BPB_ResvdSecCnt + (FAT_vars[FAT_cur_vol]->bpb->BPB_NumFATs * FAT_vars[FAT_cur_vol]->bpb->BPB_FATSz16) + LBA;

  FAT_vars[FAT_cur_vol]->SecNum = FAT_vars[FAT_cur_vol]->FirstRootDirSecNum;
  FAT_vars[FAT_cur_vol]->ClusNum = 0;
  FAT_vars[FAT_cur_vol]->FClus = 0;
  FAT_vars[FAT_cur_vol]->LastSecNum = 0;

    drv->std_open	=	FAT_open;
    drv->std_close	=	FAT_close;
    drv->std_ftell	=	FAT_ftell;
    drv->std_fseek	=	FAT_fseek;
    drv->std_feof	=	FAT_feof;
    drv->std_read	=	FAT_read;
    drv->std_write	=	FAT_write;
    drv->std_chdir	=	FAT_chdir;
    drv->std_scandir	=	FAT_scandir;
    drv->std_stat	=	FAT_stat;
    drv->std_fstat	=	FAT_fstat;
    //drv->std_init	=	FAT_init;
    drv->std_change_vol	=	FAT_change_vol;
    drv->std_quit	=	FAT_quit;
    //FS_DRIVER_ERROR error;



  return FAT_vars[FAT_cur_vol]->FATtype;
}

void FAT_register()
{

    FAT_vars = (FAT_DRIVER_VARS**) malloc(sizeof(FAT_DRIVER_VARS*));
    known_fs_drivers[0x01] = FAT_init;    
    known_fs_drivers[0x04] = FAT_init;    
    known_fs_drivers[0x06] = FAT_init;    
    known_fs_drivers[0x0b] = FAT_init;    
    known_fs_drivers[0x0c] = FAT_init;    
    known_fs_drivers[0x0e] = FAT_init;    
    known_fs_drivers[0x0f] = FAT_init;    
    known_fs_drivers[0x11] = FAT_init;    
    known_fs_drivers[0x14] = FAT_init;    
    known_fs_drivers[0x16] = FAT_init;    
    known_fs_drivers[0x1b] = FAT_init;    
    known_fs_drivers[0x1c] = FAT_init;    
    known_fs_drivers[0x1e] = FAT_init;    
}
