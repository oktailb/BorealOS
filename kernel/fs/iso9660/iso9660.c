//#include <stdio.h>
#include "string.h"
#include <drivers/fs/iso9660/Iso9660.h>
//strlen ????????
VolumeDescriptor IsoPrimaryVolume;
unsigned char *DefaultDir;
Iso9660File *input;

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int read(unsigned char drive,unsigned long longueur,unsigned long pos,void *buf)
{
  int i;
  fseek(input,2048*pos,SEEK_SET);
  i=fread(buf,longueur,2048,input);
  return ISO_ITS_OK;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int InitIso9660()
{
  int readres;
  //ide_init_driver();
  if ((input=fopen("piste01.nrg","rb"))==NULL)
    {
      printf("erreur: ne peux pas ouvrir !!!\n");
      return ISO_READ_ERR;
    }

  if ((readres = read(1,1,16,&IsoPrimaryVolume)) != 0)
    {
      printf("read a renvoye: %d\n", readres);
      return ISO_READ_ERR;
    }
  DefaultDir=(unsigned char *)malloc(2);
  DefaultDir[0]=SEPARATOR;DefaultDir[1]=0;
  return ISO_ITS_OK;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int FindLastSeparator(unsigned char *name) /*pourra etre remplacer par un strrchr*/
{
  unsigned char *res;
  res=strrchr(name,SEPARATOR);
  return ((res==NULL)?-1:res-name);
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int FindSeparator(unsigned char *name)/*pourra etre remplacer par un strchr*/
{
  unsigned char *res;
  res=strchr(name,SEPARATOR);
  return (res==NULL?-1:res-name);
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int FindDirectory(unsigned char *tname,FileDescriptor **FD)
{
  unsigned char *Buffer,*dname,*tempname;
  LittlePathTableDescriptor *LPTD;
  unsigned long PosInPathTable,TableSectorLength,TableLength;
  unsigned char BoolName,i,DirectoryNbr,PosInTree=0;
  short DirectoryLength;
  int readres;
  i=FindLastSeparator(tname);
  dname=(i==0?NULL:(unsigned char *)malloc(i+1));
  for (BoolName=1;BoolName<=i;BoolName++)dname[BoolName-1]=tname[BoolName];
  if (dname!=NULL)dname[i]=0;
  for (;BoolName<strlen(tname);BoolName++)tname[BoolName-i-1]=tname[BoolName];
  if (tname!=NULL)tname[BoolName-i-1]=0;
  TableLength=IsoPrimaryVolume.PathTableLength;
  TableSectorLength=((unsigned long)(TableLength/IsoPrimaryVolume.SectorSize))+(TableLength%IsoPrimaryVolume.SectorSize>0?1:0);
  Buffer=(unsigned char *)malloc(TableSectorLength*IsoPrimaryVolume.SectorSize);
  if ((readres = read(1,TableSectorLength,IsoPrimaryVolume.FirstSectorNumInFirstLittleEndianPathTable,Buffer)) != 0)
    {
      printf("read a renvoye: %d\n", readres);
      return ISO_READ_ERR;
    }
  LPTD=(LittlePathTableDescriptor *)Buffer;
  if (LPTD->NameLength!=1)
    {
      printf("Erreur je ne trouve pas le ROOT\n");
      return 2;
    }
  if (dname==NULL)
    {
      PosInPathTable=LPTD->FirstSectorNumber;
      LPTD=NULL;
      free(Buffer);
      return FindFileInDirectory(tname,PosInPathTable,FD);
    }
  tempname=dname;
  PosInPathTable=sizeof(LittlePathTableDescriptor)+LPTD->NameLength;
  PosInTree=1;DirectoryNbr=1;
  while((DirectoryLength=FindSeparator(tempname))!=-1)
    {
      BoolName=0;
      while (PosInPathTable<TableLength)
	{
	  LPTD=(LittlePathTableDescriptor *)(Buffer+PosInPathTable);DirectoryNbr++;
	  PosInPathTable+=sizeof(LittlePathTableDescriptor)+LPTD->NameLength;
	  if (LPTD->ParentDirectortDescriptorNumber==PosInTree)
	    {
	      if (LPTD->NameLength==DirectoryLength)
		{
		  i=0;
		  do {BoolName=(tempname[i]==(LPTD->Name)[i]);i++;} while ((BoolName)&&(i<DirectoryLength));
		  if (BoolName) break;
		}
	    }
	  else if (LPTD->ParentDirectortDescriptorNumber>PosInTree) return 4;
	}
      if (BoolName==0) return 4;
      tempname+=DirectoryLength+1;
      PosInTree=DirectoryNbr;
    }
  PosInPathTable=LPTD->FirstSectorNumber;
  LPTD=NULL;tempname=NULL;
  free(Buffer);free(dname);
  return FindFileInDirectory(tname,PosInPathTable,FD);
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int FindFileInDirectory(unsigned char *fname,unsigned long FirstSectorNumber,FileDescriptor **FD)
{
  unsigned char *Buffer=NULL,*Temp=NULL;
  unsigned long PosInDirectoryTable,MaxLength;
  unsigned short j;
  int readres;
  FileDescriptor *FDTemp=NULL;
  unsigned char BoolName,i,FileLength;
  Buffer=(unsigned char *)malloc(IsoPrimaryVolume.SectorSize);
  if ((readres = read(1,1,FirstSectorNumber,Buffer)) != 0)
    {
      printf("read a renvoye: %d\n", readres);
      return ISO_READ_ERR;
    }
  FDTemp=(FileDescriptor *)Buffer;
  if ((FDTemp->FileIDLength!=1)||((FDTemp->FileID)[0]!=0))
    {
      printf("Erreur je ne trouve pas le Root\n");
      FDTemp=NULL;
      free(Buffer);
      return ISO_ROOT_ERR;
    }
  if (fname==NULL)
    {
      (*FD)=(FileDescriptor *)malloc(FDTemp->FileDescriptorLength);
      memcpy(*FD,FDTemp,FDTemp->FileDescriptorLength);
      FDTemp=NULL;
      free(Buffer);
      return ISO_ITS_OK;
    }
  MaxLength=FDTemp->FileFirstSectorNbr;
  if (MaxLength>IsoPrimaryVolume.SectorSize!=0)
    {
      Temp=Buffer;
      Buffer=(unsigned char *)malloc(MaxLength);
      if ((readres = read(1,((unsigned long)(MaxLength/IsoPrimaryVolume.SectorSize))-1,FirstSectorNumber+1,Buffer+IsoPrimaryVolume.SectorSize)) != 0) 
	{
	  printf("read a renvoye: %d\n", readres);
	  FDTemp=NULL;
	  free(Buffer);free(Temp);
	  return ISO_READ_ERR;
	}
      for (j=0;j<IsoPrimaryVolume.SectorSize;j++)Buffer[j]=Temp[j];
      free(Temp);
    }
  PosInDirectoryTable=FDTemp->FileDescriptorLength;
  FDTemp=(FileDescriptor *)(Buffer+PosInDirectoryTable);
  if ((FDTemp->FileIDLength!=1)||((FDTemp->FileID)[0]!=1))
    {
      printf("Erreur je ne trouve pas le Parent\n");
      FDTemp=NULL;
      free(Buffer);
      return ISO_PARENT_ERR;
    }
  PosInDirectoryTable+=FDTemp->FileDescriptorLength;;
  BoolName=0;FileLength=strlen(fname);
  while (PosInDirectoryTable<MaxLength)
    {
      FDTemp=(FileDescriptor *)(Buffer+PosInDirectoryTable);
      if (FDTemp->FileDescriptorLength==0)
	{
	  FDTemp=NULL;
	  free(Buffer);
	  return ISO_FILE_FIND_ERR;
	}
      if (FDTemp->FileIDLength==FileLength+((FDTemp->FileFlag).Type?0:2))
	{
	  i=0;
	  do {BoolName=(fname[i]==(FDTemp->FileID)[i]);i++;} while ((BoolName)&&(i<FileLength));
	  if (BoolName) break;
	}
      PosInDirectoryTable+=FDTemp->FileDescriptorLength;
    }
  if (BoolName==0)
    {
      FDTemp=NULL;
      free(Buffer);
      return ISO_FILE_FIND_ERR;
    }
  (*FD)=(FileDescriptor *)malloc(FDTemp->FileDescriptorLength);
  memcpy(*FD,FDTemp,FDTemp->FileDescriptorLength);
  FDTemp=NULL;
  free(Buffer);
  return ISO_ITS_OK;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int SimplifyDirectory(unsigned char *tname)
{
  unsigned char *pos1,*pos2,*cour,pos,*res,posecr,poslect,longueur;
  longueur=strlen(tname);
  poslect=posecr=0;cour=tname;
  do    {
    pos1=strstr(cour,DIR_INUTIL);
    pos=pos1-cour;
    if((pos1==NULL)||(longueur-poslect<3))pos=longueur-poslect;
    for (;pos>0;pos--)
      {
	tname[posecr++]=tname[poslect];
	if((tname[poslect]==SEPARATOR)&&(tname[poslect+1]==SEPARATOR))
	  {
	    posecr--;
	  }
	poslect++;
      }
    poslect++;poslect++;
    cour=pos1;
    cour++;cour++;
  } while (pos1!=NULL);
  poslect=posecr;
  for (;posecr<longueur;)
    {
      tname[posecr++]=0;
    }
  longueur=poslect;
  if ((tname[longueur-2]==SEPARATOR)&&(tname[longueur-1]=='.'))
    {
      longueur--;longueur--;
      tname[longueur]=0;
    }
  res=(unsigned char *)malloc(longueur+2);
  for (posecr=0;posecr<longueur+1;){res[posecr++]=0x20;}
  res[longueur+1]=0;
  poslect=0;posecr=longueur;cour=tname;
  do    {
    pos1=strstr(cour,DIR_PARENT);
    pos=pos1-cour;
    if ((pos1==NULL)||(longueur-poslect<4)){pos=longueur-poslect;pos1=NULL;}
    for (;pos>0;pos--)
      {
	res[posecr--]=tname[poslect++];
      }
    cour=pos1;
    if (pos1!=NULL)
      {
	do {
	  pos2=strchr(res+posecr+1,SEPARATOR);
	  if (pos2==NULL)return 6;
	  posecr=pos2-res;
	  poslect++;cour++;
	} while (tname[poslect+2]=='.');
	if (tname[poslect+2]!=SEPARATOR)return 6;
      }
    poslect++;poslect++;
    cour++;cour++;
  } while(pos1!=NULL);
  for (poslect=0;poslect<longueur-posecr;poslect++)
    tname[poslect]=res[longueur-poslect];
  for (;poslect<strlen(tname);poslect++)
    tname[poslect]=0;
  free(res);
  return ISO_ITS_OK;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
Iso9660File *IsoFOpen(unsigned char *tname)
{
  Iso9660File *f=NULL;
  FileDescriptor *FD;
  int r,i,j;
  unsigned char *uname;
  r=strlen(tname);
  if (tname[0]==SEPARATOR)
    {
      uname=(unsigned char *)malloc(r+1);
      for (i=0;i<strlen(tname);i++)uname[i]=tname[i];
    }
  else
    {
      j=0;
      uname=(unsigned char *)malloc(strlen(DefaultDir)+r+1);
      for (i=0;i<strlen(DefaultDir);i++)uname[i]=DefaultDir[i];
      for (;j<strlen(tname);i++,j++)uname[i]=tname[j];
    }
  uname[i]=0;
  if ((r=SimplifyDirectory(uname)) !=0)
    {
      printf("Erreur dans SimplifyDirectory :%d\n",r);
      return 1;
      //exit(1);
    }
  if ((r=FindDirectory(uname,&FD)) !=0)
    {
      printf("Erreur dans FindDirectory :%d\n",r);
      exit(2);
    }
  f=(Iso9660File *)malloc(sizeof(Iso9660File));
  f->FirstSector=FD->FileFirstSectorNum;
  f->Open=1;
  f->Position=0;
  f->SectorNumber=0;
  f->SectorOffset=0;
  f->Size=FD->FileFirstSectorNbr;
  f->Flag=FD->FileFlag;
  free(FD);
  free(uname);
  return f;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
unsigned short IsoFClose(Iso9660File *f)
{
  f->Open = 0;
  free(f);
  return ISO_ITS_OK;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
unsigned long IsoFTell(Iso9660File *f)
{
  return f->Position;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int IsoFSeek(Iso9660File *f, unsigned long offset,unsigned char mode)
{
  switch (mode)
    {
    case 0:
      {
	if (offset>=(f->Size))
	  {
	    return 1;
	  }
	f->Position=offset;
	break;
      }
    case 1:
      {
	if ((offset+f->Position)>=(f->Size))
	  {
	    return 1;
	  }
	f->Position+=offset;
	break;
      }
    case 2:
      {
	if (offset>=(f->Size))
	  {
	    return 1;
	  }
	f->Position=f->Size-offset;
	break;
      }
    }
  f->SectorOffset=(f->Position)%(IsoPrimaryVolume.SectorSize);
  f->SectorNumber=(unsigned long)((f->Position)/(IsoPrimaryVolume.SectorSize));
  return ISO_ITS_OK;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
unsigned char IsoFEof(Iso9660File *f)
{
  return (f->Position == f->Size); // securite >=
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
unsigned short IsoFRead(void *buf, unsigned long size, unsigned long number,Iso9660File *f)
{
  unsigned long Taille,NumMax,off1,NumSect,BufferTaille,i,*Temp1,*Temp2;
  unsigned char *Buffer,*Temp;
  int readres;
  Taille=f->Size-f->Position;
  NumMax=Taille/size;
  NumMax=(NumMax>number?number:NumMax);
  Taille=NumMax*size;
  off1=f->SectorOffset+Taille;
  NumSect=off1/IsoPrimaryVolume.SectorSize;
  NumSect+=((off1%IsoPrimaryVolume.SectorSize)>0?1:0);
  BufferTaille=NumSect*IsoPrimaryVolume.SectorSize;
  Buffer=(unsigned char *)malloc(BufferTaille);
  if ((readres = read(1,NumSect,f->SectorNumber+f->FirstSector,Buffer)) != 0)
    {
      printf("read a renvoye: %d\n", readres);
      free(Buffer);
      return 0;
    }
  Temp=(unsigned char *)buf;
  for (i=0;i<(Taille&3);i++)Temp[i]=Buffer[f->Position+i];
  Temp1=(unsigned long *)(Temp+(Taille&3));
  Temp2=(unsigned long *)(Buffer+(Taille&3)+f->Position);
  for (i=0;i<(Taille>>2);i++)Temp1[i]=Temp2[i];
  f->Position+=Taille;
  f->Position-=((f->Position)==(f->Size)?1:0); //securite = >= f->Size-1 f->Position
  f->SectorOffset=(f->Position)%(IsoPrimaryVolume.SectorSize);
  f->SectorNumber+=(unsigned long)((f->Position)/(IsoPrimaryVolume.SectorSize));
  return NumMax;
}

/* ------------------- D R E A M  -  D O C -------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : Olivier Juan
** Version    : 
------------------------------------------------------------------------*/
int IsoChDir(unsigned char *newdir)
{
  int i,j,r;
  unsigned char *Temp;
  if (newdir[0]==SEPARATOR)
    {
      Temp=(unsigned char *)malloc(strlen(newdir)+1+(newdir[strlen(newdir)-1]==SEPARATOR?0:1));
      for (i=0;i<strlen(newdir);i++)Temp[i]=newdir[i];
      if (newdir[strlen(newdir)-1]!=SEPARATOR)Temp[i++]=SEPARATOR;
      Temp[i]=0;
    }
  else
    {
      j=0;
      Temp=(unsigned char *)malloc(strlen(DefaultDir)+strlen(newdir)+1+(newdir[strlen(newdir)-1]==SEPARATOR?0:1));
      
      for (i=0;i<strlen(DefaultDir);i++)
	Temp[i]=DefaultDir[i];
      
      for (;j<strlen(newdir);i++,j++)
	Temp[i]=newdir[j];
      
      if (newdir[strlen(newdir)-1]!=SEPARATOR)
	Temp[i++]=SEPARATOR;
      Temp[i]=0;
    }
  if ((i=SimplifyDirectory(Temp))!=0)
    {
      printf("Erreur dans SimplifyDirectory :%d\n",r);
      free(Temp);
      return r;
    }
  free(DefaultDir);
  DefaultDir=(unsigned char *)malloc(strlen(Temp)+1);
  
  for (i=0;i<strlen(Temp);i++)
    DefaultDir[i]=Temp[i];
  
  DefaultDir[i]=0;
  free(Temp);
  return ISO_ITS_OK;
}
