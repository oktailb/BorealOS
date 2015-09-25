#ifndef __FAT_DRV__
#define __FAT_DRV__

#include <stdlib.h>
#include <vfs/vfs.h>

/*
 *	Ces variables sont propres au driver devellope, ici la FAT(12/16/32)
 */

#define ATTR_READ_ONLY  0x01
#define ATTR_HIDDEN     0x02
#define ATTR_SYSTEM     0x04
#define ATTR_VOLUME_ID  0x08
#define ATTR_DIRECTORY  0x10
#define ATTR_ARCHIVE    0x20
#define ATTR_ANYFILE    0x37
#define ATTR_LONG_NAME  0x0F

/* End Of Chain */
#define EOC12 0x0FF8
#define EOC16 0xFFF8
#define EOC32 0x0FFFFFF8

/* Bad Cluster */
#define BAD12 0x0FF7
#define BAD16 0xFFF7
#define BAD32 0x0FFFFFF7


/* Error codes */
#define F_NOT_FOUND 	-2
#define F_NOT_OPEN 	-4
#define F_READ_ERROR 	-8

typedef struct {
        unsigned char   Name[255]       __attribute__((packed));
        unsigned char   NTRes           __attribute__((packed));
        unsigned char   Attr            __attribute__((packed));
        unsigned char   CrtTimeTenth    __attribute__((packed));
        unsigned short  CrtTime         __attribute__((packed));
        unsigned short  CrtDate         __attribute__((packed));
        unsigned short  LstAccDate      __attribute__((packed));
        unsigned short  FstClusHi       __attribute__((packed));
        unsigned short  WrtTime         __attribute__((packed));
        unsigned short  WrtDate         __attribute__((packed));
        unsigned short  FstClusLo       __attribute__((packed));
        unsigned long   FileSize        __attribute__((packed));
	} LongDir;

typedef struct
{
    unsigned char 	BS_jmpBoot[3];
    unsigned char 	BS_OEMName[8];
    unsigned short 	BPB_BytesPerSec;
    unsigned char 	BPB_SecPerClus;
    unsigned short 	BPB_ResvdSecCnt;
    unsigned char 	BPB_NumFATs;
    unsigned short 	BPB_RootEntCnt;
    unsigned short 	BPB_TotSec16;
    unsigned char 	BPB_Media;
    unsigned short 	BPB_FATSz16;
    unsigned short 	BPB_SecPerTrk;
    unsigned short 	BPB_NumHeads;
    unsigned long 	BPB_HiddSec;
    unsigned long 	BPB_TotSec32;
    
}BPB;

typedef struct {
    unsigned char   	BS_DrvNum        __attribute__((packed));
    unsigned char   	BS_Reserved1     __attribute__((packed));
    unsigned char   	BS_BootSig       __attribute__((packed));
    unsigned long   	BS_VolID         __attribute__((packed));
    unsigned char   	BS_VolLab[11]    __attribute__((packed));
    unsigned char   	BS_FilSysType[8] __attribute__((packed));

} FAT12_16; /* 26 bytes */

typedef struct {
        unsigned long   BPB_FATSz32      __attribute__((packed));
        unsigned short  BPB_ExtFlags     __attribute__((packed));
        unsigned short  BPB_FSVer        __attribute__((packed));
        unsigned long   BPB_RootClus     __attribute__((packed));
        unsigned short  BPB_FSInfo       __attribute__((packed));
        unsigned short  BPB_BkBootSec    __attribute__((packed));
        unsigned char   BPB_Reserved[12] __attribute__((packed));
        unsigned char   BS_DrvNum        __attribute__((packed));
        unsigned char   BS_Reserved      __attribute__((packed));
        unsigned char   BS_BootSig       __attribute__((packed));
        unsigned long   BS_VolID         __attribute__((packed));
        unsigned char   BS_VolLab[11]    __attribute__((packed));
        unsigned char   BS_FilSysType[8] __attribute__((packed));
	} FAT32; /* 54 bytes */

typedef struct {
        unsigned char   Name[11]        __attribute__((packed));
        unsigned char   Attr            __attribute__((packed));
        unsigned char   NTRes           __attribute__((packed));
        unsigned char   CrtTimeTenth    __attribute__((packed));
        unsigned short  CrtTime         __attribute__((packed));
        unsigned short  CrtDate         __attribute__((packed));
        unsigned short  LstAccDate      __attribute__((packed));
        unsigned short  FstClusHi       __attribute__((packed));
        unsigned short  WrtTime         __attribute__((packed));
        unsigned short  WrtDate         __attribute__((packed));
        unsigned short  FstClusLo       __attribute__((packed));
        unsigned long   FileSize        __attribute__((packed));
	} DirEntry; /* 32 bytes */

typedef struct {
        unsigned char lname1[10]        __attribute__((packed));
        unsigned char sig               __attribute__((packed));
        unsigned char attr              __attribute__((packed));
        unsigned char flags             __attribute__((packed));
        unsigned char chksum            __attribute__((packed));
        unsigned char lname2[12]        __attribute__((packed));
        unsigned short first            __attribute__((packed));
        unsigned char lname3[4]         __attribute__((packed));
	} LnDirEntry; /* 32 bytes */



/*
 *	Les variables et structures qui suivent sont typiques d'un pilote de FS
 *	Les structures *_DRIVER_VARS contiennent des variables propres au
 *	systeme de fichier traite, mais sont instanciees dans le tableau *_vars.
 *	Ainsi, un pilote doit pouvoir monter plusieurs volumes sans charger de
 *	code redondant.
 */

u8 FAT_nb_vols;
u8 FAT_cur_vol;

typedef struct {
        unsigned long fclus;  /* First Cluster of File */
	unsigned char Open;   /* 0 = Closed, 1 = Open */
	unsigned long pos;    /* Position actuelle dans le fichier */
	unsigned char Snum;   /* Secteur actuel */
	unsigned short Soff;  /* Offset dans le secteur */
	unsigned long clus;   /* cluster actuel */
	unsigned long size;
	unsigned short used;
	} FAT_file;

FAT_file** FAT_open_files_list;
u32 FAT_nb_files_open;
u32 FAT_size_of_files_list;

u32 FAT_open(u8* fname, u16 flag,u16 mode);
u8  FAT_close(u32 fd);
u32 FAT_ftell(u32 fd);
u16 FAT_fseek(u32 fd, u32 offset);
u8  FAT_feof(u32 fd);
u32 FAT_read(u32 fd, void *buf, u32 size);
u32 FAT_write(u32 fd, void *buf, u32 size);
u16 FAT_chdir(u8 *newdir);
u16 FAT_scandir(u8 * dir, struct dirent ***namelist, u16 (*select)(const struct dirent *), u16 (*compar)(const struct dirent **, const struct dirent **));

u16 FAT_stat(u8*fname, struct stat *buf);
u16 FAT_fstat(u32 filedes, struct stat *buf);
u8  FAT_init(u16 device, struct DRIVER_PROTO* drv);
u8  FAT_change_vol(u8 device);
u16 FAT_quit();

typedef struct 
{
    unsigned char Sector[512];
    BPB *bpb;
    /* Ces deux la peuvent peut-�re appartenir au driver IDE: en fait il me
     * suffit de trois variables (BytesPerSec, SecPerClus et ResvdSecCnt),
     * mais qui depenndent du disque */
    unsigned long FirstDataSector, BaseLBA;
    unsigned long SecNum, LastSecNum, ClusNum, FClus;
    unsigned short CurPos;
    unsigned char sectors_per_track, NumHeads;
    unsigned short Id;
    unsigned long FirstRootDirSecNum, RootDirSectors;
    unsigned char *name;
    unsigned char Attr;
    unsigned long EOC;    /* End of cluster chain */
    unsigned char FATtype;

} FAT_DRIVER_VARS;

FAT_DRIVER_VARS** FAT_vars;

#endif
