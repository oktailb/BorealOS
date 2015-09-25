#ifndef __VFS__
#define __VFS__

#include "drivers/block/block/block.h"
//#include "serverText.h"

//#include "stddef.h"
#include "string.h"

#define VFS_ERR_UNKNOWN_DRIVER 			-1
#define VFS_ERR_UNKNOWN_DEVICE 			-2
#define VFS_ERR_NO_MEDIA 			-4
#define VFS_ERR_UNSUPPORTED_FEATURE 		-8
#define VFS_ERR_UNKNOWN 			-16

#define VFS_WARNING_NO_FSTAB 			0
#define VFS_WARNING_INVALID_MAGIC_NUMBER 	1

#define NB_KNOWN_FS 256

#define 	O_RDONLY	1
#define 	O_WRONLY	2
#define 	O_RDWR		4
#define 	O_CREAT		8
#define 	O_EXCL		16
#define 	O_NOCTTY	32
#define 	O_TRUNC		64
#define 	O_APPEND	128
#define 	O_NONBLOCK	256
#define 	O_NDELAY	512
#define 	O_SYNC		1024
#define 	O_NOFOLLOW	2048
#define 	O_DIRECTORY	4096
#define 	O_LARGEFILE	8192

#define		S_IFMT    	0170000   	// bitmask for the file type bitfields
#define		S_IFSOCK   	0140000   	// socket
#define		S_IFLNK    	0120000   	// symbolic link
#define		S_IFREG    	0100000   	// regular file
#define		S_IFBLK    	0060000   	// block device
#define		S_IFDIR    	0040000   	// directory
#define		S_IFCHR    	0020000   	// character device
#define		S_IFIFO    	0010000   	// fifo
#define		S_ISUID    	0004000   	// set UID bit
#define		S_ISGID    	0002000   	// set GID bit (see below)
#define		S_ISVTX    	0001000   	// sticky bit (see below)
#define		S_IRWXU    	0000700     	// mask for file owner permissions
#define		S_IRUSR    	0000400     	// owner has read permission
#define		S_IWUSR    	0000200     	// owner has write permission
#define		S_IXUSR    	0000100     	// owner has execute permission
#define		S_IRWXG    	0000070     	// mask for group permissions
#define		S_IRGRP    	0000040     	// group has read permission
#define		S_IWGRP    	0000020     	// group has write permission
#define		S_IXGRP    	0000010     	// group has execute permission
#define		S_IRWXO    	0000007     	// mask for permissions for others (not in group)
#define		S_IROTH    	0000004     	// others have read permission
#define		S_IWOTH    	0000002     	// others have write permisson
#define		S_IXOTH    	0000001     	// others have execute permission

#define		S_ISREG(m)			// is it a regular file?
#define		S_ISDIR(m)  			// directory?
#define		S_ISCHR(m)  			// character device?
#define		S_ISBLK(m)  			// block device?
#define		S_ISFIFO(m) 			// fifo?
#define		S_ISLNK(m)  			// symbolic link? (Not in POSIX.1-1996.)
#define		S_ISSOCK(m) 			// socket? (Not in POSIX.1-1996.)

struct stat {
    u16		st_dev;      /* device */
    u32		st_ino;      /* inode */
    u16        	st_mode;     /* protection */
    u32       	st_nlink;    /* number of hard links */
    u32         st_uid;      /* user ID of owner */
    u32         st_gid;      /* group ID of owner */
    u32         st_rdev;     /* device type (if inode device) */                  
    u32         st_size;     /* total size, in bytes */
    unsigned long st_blksize;  /* blocksize for filesystem I/O */
    unsigned long st_blocks;   /* number of blocks allocated */
    u32        st_atime;    /* time of last access */
    u32        st_mtime;    /* time of last modification */
    u32        st_ctime;    /* time of last change */
};

struct dirent
  {
    unsigned short int d_reclen;
    unsigned char d_type;
    char d_name[256];		/* We must not include limits.h! */
  };


struct PATH
{

    struct PATH* next;
    u8* dir;
    u16 level;
};

typedef struct PATH PATH_LIST;

typedef struct
{
    unsigned no_media:1;
    unsigned write_err:1;
    unsigned read_err:1;
    unsigned fs_err:1;
    unsigned unknow:4;

}FS_DRIVER_ERROR;

struct DRIVER_PROTO /* toutes les fonctions d'acces aux donnees de maniere standard, quelque soit le pilote => a initialiser par celui-ci ! */
{
    u32 (*std_open)(u8* fname, u16 flag,u16 mode);
    u8  (*std_close)(u32 fd);
    u32 (*std_ftell)(u32 fd);
    u16 (*std_fseek)(u32 fd, u32 offset);
    u8  (*std_feof)(u32 fd);
    u32 (*std_read)(u32 fd, void *buf, u32 size);
    u32 (*std_write)(u32 fd, void *buf, u32 size);
    u16 (*std_chdir)(u8 *newdir);
    u16 (*std_scandir)(u8 * dir, struct dirent ***namelist, u16 (*select)(const struct dirent *), u16 (*compar)(const struct dirent **, const struct dirent **));
    
u16 (*std_stat)(u8*fname, struct stat *buf);
    u16 (*std_fstat)(u32 filedes, struct stat *buf);
    u8  (*std_init)(u16 device, struct DRIVER_PROTO* drv);
    u8  (*std_change_vol)(u8 device);
    u16 (*std_quit)();
    u8 vol_descriptor;
    FS_DRIVER_ERROR error;
    
};

typedef struct DRIVER_PROTO DRIVER;

#include "filesystems.h"

struct MOUNT /* tout ce qui definit un point de montage */
{

    u8* path; /*le repertoire de montage */
    u16 device; /* le peripherique sur le quel on peut le trouver */
    u8* type; /* le type de FS */
    DRIVER* driver; /* l'acces aux fonctions du pilote */
    
    unsigned amovible:1; /* est il amovible ? */
    unsigned open:1; /* si oui, la trappe est elle fermee ? */
    unsigned present:1; /* est il physiquement present ? */
    unsigned lock:1; /* le tiroir est il bloque ou peut on sortir le media ? */
    unsigned used:1;
    unsigned FS_initialised:1;
    unsigned unused:3;
        
};

typedef struct MOUNT MOUNT_POINT;

typedef struct
{
    u8* name;		// nom complet du fichier
    u8* path;		// PATH du fichier
    MOUNT_POINT* mount;	// volume au quel il appartient le fichier
    u16 used;		// cet emplacement est il dispo ?
    u8 lock;		// fichier utilise ?
    u32 fd;		// num du descripteur de fichier propre au pilote de FS
    
}file_descriptor;

u8 init_vfs(u16 device, u8* type);

u8 mount(u8* path, u16 device, u8* type, u8* options);
u8 umount(MOUNT_POINT vol);
u32 open(u8* fname,u16 flag,u16 mode);
u8 close(u32 fd);
u32 ftell(u32 fd);
u16 fseek(u32 fd, u32 offset);
u8 eof(u32 fd);
u32 read(u32 fd, void *buf, u32 size);
u32 write(u32 fd, void *buf, u32 size);
u16 chdir(u8 *newdir);
u16 scandir(u8 * dir, struct dirent ***namelist, u16 (*select)(const struct dirent *), u16 (*compar)(const struct dirent **, const struct dirent **));

u16 stat(u8*fname, struct stat *buf);

u16 fstat(u32 filedes, struct stat *buf);
PATH_LIST* hache(u8* path);
PATH_LIST* next_path(PATH_LIST* path);

u16 search_next_mount();
void warning(u8 warning);
u32 search_next();
u8 simplify_path(u8* path);
u8 vfs_strcmp(u8* ch1,u8* ch2);
u16 select_mount(u8* path);

u32 nb_files_open;
u32 size_of_files_list;
file_descriptor** open_files_list;	// liste des fichiers ouverts

#endif
