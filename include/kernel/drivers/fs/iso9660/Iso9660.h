#include <stdlib.h>

#define SEPARATOR '\\'
#define DIR_INUTIL "\\.\\"
#define DIR_PARENT "\\.."
#define ISO_ITS_OK 0
#define ISO_READ_ERR 1
#define ISO_ROOT_ERR 2
#define ISO_PARENT_ERR 3
#define ISO_DIRECT_ERR 4
#define ISO_FILE_FIND_ERR 5
#define ISO_SYNTAX_ERR 6

#define SEEK_SET	0
#define SEEK_CUR	1
#define SEEK_END	2

typedef struct
	{
	unsigned char Year[4];//ex: "2000"
    unsigned char Month[2];//"01"=January, "02"=February, etc.
    unsigned char Day[2];//"01" to "31"
    unsigned char Hour[2];//from "00" to "23"
    unsigned char Minute[2];//from "00" to "59"
    unsigned char Second[2];//from "00" to "59"
    unsigned char Hundredths[2];//from "00" to "99"
    signed char DecayGreenWichMeanTime;/*      offset from Greenwich Mean Time, in 15-minute intervals,
                 as a twos complement signed number, positive for time
                 zones east of Greenwich, and negative for time zones
                 west of Greenwich*/
    } __attribute__((packed)) DateNTimeDescriptor ; /*17 bytes*/
        
typedef struct
	{
	unsigned Hidden:1;//0 for a normal file, 1 for a hidden file
    unsigned Type:1;//0 for a file, 1 for a directory
    unsigned Associated:1;// 0 [1 for an associated file]
    unsigned FormatRecord:1;// 0 [1 for record format specified]
    unsigned Permissions:1;// 0 [1 for permissions specified]
    unsigned reserved:2;//reserved = 0
    unsigned AnotherFileDescriptor:1;// 0 [1 if not the final record for the file]
    } __attribute__((packed)) IsoFlag; /*1 byte*/

typedef struct
	{
	unsigned char FileDescriptorLength;//the number of bytes in the record = R(which must be even)
    unsigned char ExtendedAttributeDescriptorLength;//number of sectors in extended attribute record =unused
    unsigned long FileFirstSectorNum;//number of the first sector of file data or directory =0 if empty file (as a both endian double word)
    unsigned long FileFirstSectorNumInv;//Suite
    unsigned long FileFirstSectorNbr;//number of bytes of file data or length of directory, excluding the extended attribute record, (as a both endian double word)
    unsigned long FileFirstSectorNbrInv;//Suite
    unsigned char RecordYear;//number of years since 1900
    unsigned char RecordMonth;//month, where 1=January, 2=February, etc.
    unsigned char RecordDay;//day of month, in the range from 1 to 31
    unsigned char RecordHour;//hour, in the range from 0 to 23
    unsigned char RecordMinute;//minute, in the range from 0 to 59
    unsigned char RecordSecond;//seconde in the range from 0 to 59 (for DOS this is always an even number)
    signed char RecordOffsetGreenwichMeanTime;//offset from Greenwich Mean Time, in 15-minute intervals, as a twos complement signed number, positive for time zones east of Greenwich, and negative for time zones west of Greenwich (DOS ignores this field)
    IsoFlag FileFlag;
    unsigned char FileUnitSize;//file unit size for an interleaved file =0 if no interleaved
    unsigned char InterleaveGapSize;//interleave gap size for an interleaved file =0 if no interleave
    unsigned short VolumeSeqNum;//volume sequence number of the parent volume (as a both endian word)
    unsigned short VolumeSeqNumInv;//Suite
    unsigned char FileIDLength;//the identifier length =N
    unsigned char FileID[0];//identifier =0
    unsigned char PaddingByte;//padding byte: if N is even, P = 1 and this field contains a zero; if N is odd, P = 0 and this field is omitted
	} __attribute__((packed)) FileDescriptor; /*34 bytes*/

typedef struct
	{
	unsigned char VolumeDescriptorNbr;
	unsigned char CD1[5];//"CD001"
	unsigned char VolumeDescriptorVersion;//Version of the Volume Descriptor =1
	unsigned char unused1;//unused
	unsigned char SystemID[32];//system identifier
	unsigned char VolumeID[32];//volume identifier
	unsigned char unused2[8];//unused
	unsigned long TotalNbrSector;//total number of sectors (as a both endian double word)
	unsigned long TotalNbrSectorInv;//Suite
	unsigned char unused3[32];//unused
	unsigned short VolumeSetSize;//volume set size =1 (as a both endian word)
	unsigned short VolumeSetSizeInv;//Suite
	unsigned short VolumeSeqNum;//volume sequence number size =1 (as a both endian word)
	unsigned short VolumeSeqNumInv;//Suite
	unsigned short SectorSize;//the sector size =2048 (as a both endian word)
	unsigned short SectorSizeInv;//Suite
	unsigned long PathTableLength;//path table length in bytes (as a both endian double word)
	unsigned long PathTableLengthInv;//Suite
	unsigned long FirstSectorNumInFirstLittleEndianPathTable;//number of first sector in first little endian path table (as a little endian double word)
	unsigned long FirstSectorNumInSecondLittleEndianPathTable;//number of first sector in second little endian path table =0 if no second table (as a little endian double word)
	unsigned long FirstSectorNumInFirstBigEndianPathTableInv;//number of first sector in first big endian path table (as a big endian double word)
	unsigned long FirstSectorNumInSecondBigEndianPathTableInv;//number of first sector in second big endian path table =0 if no second table (as a big endian double word)
    FileDescriptor RootDirectoryDrescriptor;//root directory record
    unsigned char VolumeSetID[128];//volume set identifier =20 if no
    unsigned char PublisherID[128];//publisher identifier =20 if no
    unsigned char DataPreparerID[128];//data preparer identifier =20 if no
    unsigned char ApplicationID[128];//application identifier =20 if no
    unsigned char CopyrightFileID[37];//copyright file identifier =20 if no file
    unsigned char AbstractFileID[37];//abstract file identifier =20 if no Copyright File
    unsigned char BibliographicalFileId[37];//bibliographical file identifier =20 if no Copyright File
    DateNTimeDescriptor VolumeCreationDateTime;//date and time of volume creation
    DateNTimeDescriptor RecentModifDateNTime;//date and time of most recent modification
    DateNTimeDescriptor VolumeObsoleteDateNTime;//date and time when volume expires
    DateNTimeDescriptor VolumeEffectiveFateNTime;//date and time when volume is effective
    unsigned char FileStructVersion;//File structure version =1
    unsigned char reserved;//reserved=0
    unsigned char ApplicationUseData[512];//reserved (usually zeros)
    unsigned char reserveds[653];//reserved=0
    } __attribute__((packed)) VolumeDescriptor ; /*2048 bytes*/

typedef struct
	{
	unsigned char NameLength;// N, the name length (or 1 for the root directory)
    unsigned char ExtendAttributeDescriptorLengthInv;//0 [number of sectors in extended attribute record]
    unsigned long FirstSectorNumberInv;//number of the first sector in the directory, as a double word
    unsigned short ParentDirectortDescriptorNumber;//number of record for parent directory (or 1 for the root directory), as a word; the first record is number 1, the second record is number 2, etc.
    unsigned char Name[0];//name (or 0 for the root directory)
    unsigned char PaddingByte;//padding byte: if N is odd, this field contains a zero; if N is even, this field is omitted
    } __attribute__((packed)) BigPathTableDescriptor;/*8 bytes + N*/

typedef struct
	{
	unsigned char NameLength;// N, the name length (or 1 for the root directory)
    unsigned char ExtendAttributeDescriptorLength;//0 [number of sectors in extended attribute record]
    unsigned long FirstSectorNumber;//number of the first sector in the directory, as a double word
    unsigned short ParentDirectortDescriptorNumber;//number of record for parent directory (or 1 for the root directory), as a word; the first record is number 1, the second record is number 2, etc.
    unsigned char Name[0];//name (or 0 for the root directory)
    unsigned char PaddingByte;//padding byte: if N is odd, this field contains a zero; if N is even, this field is omitted
	} __attribute__((packed)) LittlePathTableDescriptor;/*8 bytes + N*/

typedef struct {
    unsigned long FirstSector;  /* First Sector of File */
	unsigned char Open;   /* 0 = Closed, 1 = Open */
	unsigned long Position;    /* Position actuelle dans le fichier */
	unsigned long SectorNumber;   /* Secteur actuel */
	unsigned short SectorOffset;  /* Offset dans le secteur */
	unsigned long Size; /* Taille en Octet*/
	IsoFlag	Flag;
	} __attribute__((packed)) Iso9660File;


int InitIso9660();
int FindLastSeparator(unsigned char *name); /*pourra etre remplacer par un strrchr*/
int FindSeparator(unsigned char *name);/*pourra etre remplacer par un strchr*/
int FindDirectory(unsigned char *tname,FileDescriptor **FD);
int FindFileInDirectory(unsigned char *fname,unsigned long FirstSectorNumber,FileDescriptor **FD);
Iso9660File *IsoFOpen(unsigned char *tname);
unsigned short IsoFClose(Iso9660File *f);
unsigned long IsoFTell(Iso9660File *f);
int IsoFSeek(Iso9660File *f, unsigned long offset,unsigned char mode);
unsigned char IsoFEof(Iso9660File *f);
unsigned short IsoFRead(void *buf, unsigned long size, unsigned long number,Iso9660File *f);
int IsoChDir(unsigned char *newdir);
