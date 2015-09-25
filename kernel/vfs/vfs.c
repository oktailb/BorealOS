/* Code du VFS : C */

//#define FLOPPY_DRIVER
//#define IDE_HD_DRIVER
//#define IDE_CD_DRIVER

#include "stdlib.h"

#include <vfs/vfs.h>
u8* current_path;

u8* known_fs[256]={
/*	    0		   1		   2		   3		   4		   5		   6		   7		   8		   9		   a		   b	   c		   d	   e		   f	*/
/*  0  */ "null"	,"fat"		,"xenix"	,"xenix"	,"fat"		,"extend"	,"fat"		,"ntfs"		,"aix"		,"aix"		,"ez-boot"	,"vfat"	,"vfat"		,"?"	,"vfat"		,"vfat",
/*  1  */ "opus"	,"fat"		,"?"		,"?"		,"?"		,"fat"		,"?"		,"fat"		,"ntfs"		,"swap"		,"?"		,"?"	,"vfat"		,"vfat"	,"vfat"		,"?",
/*  2  */ "?"		,"?"		,"?"		,"?"		,"nec"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"?",
/*  3  */ "?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"plan9"	,"?"		,"?"	,"resume"	,"?"	,"?"		,"?",
/*  4  */ "venix"	,"PPCboot"	,"sfs"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"qnx"	,"qnx"		,"qnx",
/*  5  */ "ontrack"	,"ontrack"	,"cp/m"		,"ontrack"	,"ontrack"	,"ez-drive"	,"golen-bow"	,"?"		,"?"		,"?"		,"?"		,"?"	,"priam-edisk"	,"?"	,"?"		,"?",
/*  6  */ "?"		,"speedstor"	,"?"		,"hurd"		,"novell"	,"novell"	,"?"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"?",
/*  7  */ "mult"	,"?"		,"?"		,"?"		,"?"		,"pc/ix"	,"?"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"?",
/*  8  */ "minix"	,"minix"	,"linux-swap"	,"fat"		,"extend-linux"	,"vol"		,"vol"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"lvm"		,"?",
/*  9  */ "?"		,"?"		,"?"		,"amoeba"	,"amoebabbt"	,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"bsd/os",
/*  a  */ "hibernation"	,"?"		,"?"		,"?"		,"?"		,"bsd/386"	,"openbsd"	,"nextstep"	,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"?",
/*  b  */ "?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"bsdi"		,"bsdi-swap"	,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"?",
/*  c  */ "?"		,"drdos"	,"?"		,"?"		,"drdos"	,"?"		,"drdos"	,"syrinx"	,"?"		,"?"		,"?"		,"?"	,"?"		,"?"	,"?"		,"?",
/*  d  */ "?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"data"		,"cp/m"	,"?"		,"?"	,"?"		,"?",
/*  e  */ "?"		,"fat"		,"?"		,"fat"		,"speedstor"	,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"beos"	,"?"		,"?"	,"?"		,"?",
/*  f  */ "?"		,"speedstor"	,"fat"		,"?"		,"speedstor"	,"?"		,"?"		,"?"		,"?"		,"?"		,"?"		,"?"	,"?"		,"auto"	,"lanstep"	,"bbt"
};

MOUNT_POINT** mounts; /* l'ensemble des points de montages => a remplacer par une liste chainee, se sera plus simple pour les changements */

u16 nb_mounts;
u16 next_mount;

file_descriptor** open_files_list;	// liste des fichiers ouverts


#include <drivers/block/devices.h>

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : warning
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.01
------------------------------------------------------------------------*/

void warning(u8 warning)
{
    return;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : search_next
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : entier 32 bits contenant la valeur du premier emplacement
** 		de file descriptor dispo.
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.01
------------------------------------------------------------------------*/

u32 search_next()
{
    int i;
    
    for(i=0;i<nb_files_open;i++)
    {
	if(!open_files_list[i]->used)return i;
    }
    
    return nb_files_open;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : simplify_path
** Entree(s)  : u8* path
** Sortie(s)  : 
** Val de ret : 
** Sujet      : Simplifie un PATH afin de lui retirer toutes ses subtilits
**		du genre . et ..
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.01
------------------------------------------------------------------------*/

u8 simplify_path(u8* path)
{
    int i=0,len,count,decal=0;
    int ref[32];
    
    len = strlen(path);
    if(path[0] == '/'){strcat(path,"/");strcat(path,current_path);}
    
    for(i=0;i<len;i++)
    {
	if(path[i] == '/')
	{
	    count++;
	    ref[count] = i;
	}
    }
    
    for(i=0;i<count;i++)
    {
	if(path[i+1] == '.')
	{
	    if(path[i+2] == '.')
	    {
		decal+=2;
		memcpy(path+ref[count+1]-decal,path+ref[count-1],len+1-ref[count+1]);
	    }
	    else
	    {
		decal+=1;
		memcpy(path+ref[count+1]-decal,path+ref[count]-decal,len+1-ref[count+1]);
	    }
	}
    }
    return 0;
}


/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : vfs_strcmp
** Entree(s)  : u8* ch1, *ch2
** Sortie(s)  : Emplacement ou les chaines different
** Val de ret : i
** Sujet      : Compare deux chaines et indique le point de derivation
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 1
------------------------------------------------------------------------*/

u8 vfs_strcmp(u8* ch1,u8* ch2)
{
    int i=0;
    while( ch1[i] && ch2[i] )
    {
	i++;
	if(ch1[i] != ch2[i])return i;
    }
    return i;
}


/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : select_mount
** Entree(s)  : u8* path
** Sortie(s)  : int indiquant le numero du point de montage 
**		concernant le path
** Val de ret : 0 si aucune correspondance ( donc la racine ... )
** Sujet      : Permet de savoir a quel point de montage se refere un chemin
** Dependance : vfs_strcmp
** Auteur     : LECOQ Vincent
** Version    : 1
------------------------------------------------------------------------*/

u16 select_mount(u8* path)
{
    u16 i=0,res=0,ret=0,tmp=0;
    
    for(i=1;i<nb_mounts;i++)
    {
	tmp = vfs_strcmp(path,mounts[i]->path);
	if(tmp > res)
	{
	    res=tmp;
	    ret=i;
	}
    }
    return ret;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : open
** Entree(s)  : chaine contenant le nom du fichier
**		informations d'ouverture du fichier
**		mode d'access unix
** Sortie(s)  : numero de file descriptor correspondant
** Val de ret : -1 si erreur, 
** Sujet      : 
** Dependance : strcpy, search_next, simplify_path, mounts ...!!!!!!!!!
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u32 open(u8 *fname, u16 flag, u16 mode)
{
    u16 m=0,fd;
    u32 res,dif;











/*==========================================================================
 *
 *	Gros PB sur nb_files_open et open_files_list :
 *	si on ferme un fichier qui n'est pas le dernier de la liste, 
 *	on cree un trou dans la dite liste => nb_files_open ne correspond 
 *	donc plus a la taille de la liste ! ...
 *
 *=========================================================================*/

// corrige !







    fd = search_next();		// recherche le prochain file descriptor dispo
    if(fd == size_of_files_list)
    {
	realloc(open_files_list, sizeof(file_descriptor*)*(fd+1));
	open_files_list[fd] = (file_descriptor*) malloc(sizeof(file_descriptor));
	size_of_files_list++;
    }
    open_files_list[fd]->used = 1;	// le declare occupe
    nb_files_open++;		// repercute le nb de fichiers ouverts
    simplify_path(fname);	// corrige les eventuelles subtilites du path
    
    m = select_mount(fname);	// trouve le point de montage auquel il appartient

// comment tester si un media est present ?

    blockGetSize(mounts[m]->device,&res);
    if( (res<0) || !(mounts[m]->FS_initialised) )
    {
    /*    
	if(mounts[m]->disk_info->support.Inject)
	{
	    blockInject(mounts[m]->device);
	}
	else
	{
	    return (-VFS_ERR_NO_MEDIA);
	}*/
//pedef struct __disk_supported
//
//bool init;               /* Initialisation */
//bool read;               /* Lecture */
//bool write;              /* criture */
//bool getInf;             /* Obtenir des informations */
//bool getSize;            /* Obtenir la taille */
//bool lock;               /* Bloquer */
//bool unlock;             /* D�loquer */
//bool isChanged;          /* Savoir si le disque a chang�*/
//diskSupportedFeatures_t;
    }

    if(blockIsChanged(mounts[m]->device) || !(mounts[m]->FS_initialised))
    {
	mounts[m]->driver->std_init(mounts[m]->device, (struct DRIVER_PROTO*) mounts[m]->driver);
    }

    dif=vfs_strcmp(mounts[m]->path,fname);	// trouve le point de separation entre le path et le mount
    mounts[m]->driver->std_change_vol(mounts[m]->driver->vol_descriptor);
    res=mounts[m]->driver->std_open(fname+dif,flag,mode); // ouvre le fichier
    if(res<0)return res;
    // rempli les structures du file des ...
    strcpy(open_files_list[fd]->name,fname);
    strcpy(open_files_list[fd]->path,fname);
    open_files_list[fd]->mount = mounts[m];
    open_files_list[fd]->lock = 0;
    open_files_list[fd]->fd = res;
    return 0;    
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u8 close(u32 fd)
{
    if(!open_files_list[fd]->used)return -1; // faudrait voir a fermer un fichier qui est bien ouvert ...
    open_files_list[fd]->mount->driver->std_close(fd); // on ferme !
    open_files_list[fd]->used = 0;		// c'est libre 
    nb_files_open--;
    return 0;    
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : xc
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u32 ftell(u32 fd)
{
    return (open_files_list[fd]->mount->driver->std_ftell(fd));
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u16 fseek(u32 fd, u32 offset)
{
    return (open_files_list[fd]->mount->driver->std_fseek(fd,offset));
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u8 feof(u32 fd)
{
    return (open_files_list[fd]->mount->driver->std_feof(open_files_list[fd]->fd));
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u32 read(u32 fd, void *buf, u32 size)
{
    return (open_files_list[fd]->mount->driver->std_read(fd,buf,size));
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u32 write(u32 fd, void *buf, u32 size)
{
    return(open_files_list[fd]->mount->driver->std_write(fd,buf,size));
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : 
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 
------------------------------------------------------------------------*/

u16 chdir(u8 *newdir)
{
    u8* atomic_dir;
    u16 i,j,len;
    u8 test_char;
    PATH_LIST* path;
        
    /* NB : gerer les options du point de montage !!!! " */
    /* PB : et si les points de montages sont imbriques ??? */
    
    if (*newdir=='/')/* L'appel part de la racine ... */
    {
        
	for(j=1;i<nb_mounts;j++) // on parcours tout les points de montages connus sauf la racine
        {
	    len=strlen(mounts[j]->path)+1;	// on recupere le point de montage, le +1 est pour le \0 en fin de chaine
            for(i=0;i<len;i++);			// on a besoin du debut seulement de la chaine afin de la comparer
	        atomic_dir[i] = newdir[i];	// on la recupere donc ...
		
	    atomic_dir[i] = '\0';		// le +1 du len est remplace par un '\0' ...
		
	    if(!strcmp(atomic_dir,mounts[j]->path)) // si la comparaison est ok, c'est que l'on doit s'addresser au volume correspondant.
	    {
		/* prendre new_dir a partir de len et le decomposer en chdir au pilote charge du volume.*/
		
		//while(test_char!='\0')/* tant que le chemin comporte des elements*/
		//{
		//    for(i=len;test_char!='/';i++) /* pour chaque element */
	    	//    {
		//        test_char = atomic_dir[i-len] = *newdir; /* recuperer l'element */
		//	newdir++;
	    	//    }
		//    atomic_dir[i-len+1]='\0';	
		//
		//    mounts[j]->driver.std_chdir(atomic_dir); /* descendre dans celui-ci */
		//}
		path=hache(newdir+len);
		
		while(path->next){mounts[j]->driver->std_chdir(path->dir);path=next_path(path);}
		
		strcpy(current_path,newdir-len);
		
		return 1;
	    }
	    else				// sinon c'est que l'on reste sur le volume racine.
	    {
		/* prendre le chdir du volume racine */
		
		//while(test_char!='\0')
		//{
		//    for(i=len;test_char!='/';i++) /* pour chaque element */
	    	//	{
		//    	    test_char = atomic_dir[i-len] = *newdir; /* recuperer l'element */
		//	    newdir++;
		//	}
		//	atomic_dir[i-len+1]='\0';	
		//
		//    mounts[0]->driver.std_chdir(atomic_dir); /* descendre dans celui-ci */
		//}

		path=hache(newdir+len);
		
		while(path->next){mounts[0]->driver->std_chdir(path->dir);path=next_path(path);}
			
		strcpy(current_path,newdir-len);
		
		return 1;    
	    }
        }
	
    }
    else /* L'appel part de current_path */
    {
	while(test_char!='\0')
	{
	    len = strlen(newdir);
    	    for(i=len;test_char!='/';i++) /* pour chaque element de newdir */
	    {
    		test_char = atomic_dir[i-len] = *newdir; /* recuperer l'element */
		newdir++;
	    }
	    atomic_dir[i-len+1]='\0';
	    if(!strcmp(atomic_dir,"."))
	    {
//		newdir+=2;
	    }
	    else
	    {
		if(!strcmp(atomic_dir,".."))
		{
		    len=strlen(current_path);
		    for(i=len;test_char!='/';i--)
		    {
			current_path[i]='\0';
		    }
		}
		else
		{
		    strcat(current_path,"/");
		    strcat(current_path,atomic_dir);
		}
	    }	
	}
	chdir(current_path);
    }
    
    return 0;    
    
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : scandir
** Entree(s)  :  
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/

u16 scandir(u8 * dir, struct dirent ***namelist, u16 (*select)(const struct dirent *), u16 (*compar)(const struct dirent **, const struct dirent **))
{
    u16 res,m,dif;
    simplify_path(dir);	// corrige les eventuelles subtilites du path
    
    m = select_mount(dir);	// trouve le point de montage auquel il appartient
    dif=vfs_strcmp(mounts[m]->path,dir);	// trouve le point de separation entre le path et le mount
    res=mounts[m]->driver->std_scandir(dir+dif,namelist,select,compar); // ouvre le fichier
    return res;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : stat
** Entree(s)  : u8* fname, nom du fichier et struct stat *buf, buffer 
**     		recevant les infos ...
** Sortie(s)  : 
** Val de ret : -1 si erreur, 0 sinon
** Sujet      : statistiques sur un fichier
** Dependance : vfs_strcmp, simplify_path
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/


u16 stat(u8*fname, struct stat *buf)
{
    u16 res,m,dif;
    simplify_path(fname);	// corrige les eventuelles subtilites du path
    
    m = select_mount(fname);	// trouve le point de montage auquel il appartient
    dif=vfs_strcmp(mounts[m]->path,fname);	// trouve le point de separation entre le path et le mount
    res=mounts[m]->driver->std_stat(fname+dif,buf); // ouvre le fichier
    return res;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : fstat
** Entree(s)  : u32 filedes et struct stat *buf
** Sortie(s)  : -1 si erreur, 0 sinon
** Val de ret : 
** Sujet      : Decomposition d'un PATH en elements simples
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/


u16 fstat(u32 fd, struct stat *buf)
{
    return (open_files_list[fd]->mount->driver->std_fstat(fd,buf));
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : hache
** Entree(s)  : u8* path
** Sortie(s)  : PATH_LIST
** Val de ret : 
** Sujet      : Decomposition d'un PATH en elements simples
** Dependance : strlen
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/

PATH_LIST* hache(u8* path)
{
    PATH_LIST* liste;
    PATH_LIST* retour = liste;
    
    u16 i=0,j=0;
    if(*path == '/'){path++;}
    for(i=0;*path!='\0';i++)
    {
	while(*path!='/')
	{
	    liste->dir[j] = *path;
	    path++;j++;
	}
	liste->dir = '\0';
	liste->level = i;
	liste=liste->next;
	j=0;
    }    
    liste->next=0;
    return retour;
}

PATH_LIST* next_path(PATH_LIST* path)
{
    return path->next;
}

void delete_next_path(PATH_LIST* path)
{
    path->next=path->next->next;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : search_next_mount
** Entree(s)  : 
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/

u16 search_next_mount()
{
    int i;
    for(i=0;i<nb_mounts;i++)
    {
	if(!mounts[i]->used)return i;
    }
    return -1;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : mount
** Entree(s)  : MOUNT_POINT vol
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/

u8 mount(u8* path, u16 device, u8* type, u8* options)
{
    int num,i,ok;
    MOUNT_POINT* mnt;
    u8 amovible;
    
    num = search_next_mount();
    		
    for(i=0;i<NB_KNOWN_FS;i++)
    {
	if(!strcmp(type,known_fs[i])){ok=1;break;}
    }
    if(ok!=1){return VFS_ERR_UNKNOWN_DRIVER;}

    if(amovible)
    {
	mnt->amovible = 1;
	mnt->FS_initialised = 0;
    }
    else
    {
	mnt->amovible = 0;
	known_fs_drivers[i](device,mnt->driver);
	mnt->FS_initialised = 1;
    }
	
    return 0;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : umount
** Entree(s)  : MOUNT_POINT vol
** Sortie(s)  : 
** Val de ret : 
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/

u8 umount(MOUNT_POINT vol)
{
    return 0;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : do_system_mount
** Entree(s)  : les descripteurs de fichiers du fstab et du mtab
** Sortie(s)  : 
** Val de ret : 0 si OK, -1 si Pb
** Sujet      : 
** Dependance : 
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/
int parse_fstab(u32 fstab, u32 mtab)
{
    char* data;
    char* base;
    u32 i=0, j=0, k=0;
    char* args[4];
    
    u32 size=1;
    data = (char*) malloc(sizeof(char));
    base = data;
    while( read(fstab, data, sizeof(char)) != (-1) )
    {
	data++;
	size++;
	realloc(data, size); // haaeemm ... et si malloc shifte data ailleurs en mem pour raisons de fragmentations ?
    }
    
    for(i=0;i<size;i++)
    {
	if( (base[i] == ' ') | (base[i] == '\n') | (base[i] == '\t') | (base[i] == '\r') )
	{
	    base[i] = '\0';
	}
	else
	{
	    if(base[i-1] == '\0')
	    {
		args[j] = &base[i];
		j++;
		if(j==4)
		{
		    j=0;
		    for(k=0;dev_names[k].num==0;k++)
		    {
			if(!strcmp(dev_names[k].name, args[1]))
			    break;
		    }
		    if(dev_names[k].num==0)
		    {
			//printf("Impossible de monter %s, peripherique non enregistre !\n", args[1]);
		    }
		    else
		    {
			//printf("Montage de %s sur %s\n",args[1], args[2]);
			mount(args[0], dev_names[k].num, args[2], args[3]);
		    }
		}
	    }
	}
    }
    return 0;
}

/* --------------------- O W L  -  D O C ---------------------------------
** Nom        : init_vfs
** Entree(s)  : un numero de device et une chaine decrivant le type de FS
** Sortie(s)  : code d'erreur ou de succes
** Val de ret : 0 si OK, -1 si Pb
** Sujet      : 
** Dependance : un Minimum serait le mieux
** Auteur     : LECOQ Vincent
** Version    : 0.1
------------------------------------------------------------------------*/

u8 init_vfs(u16 device, u8* type)
{
	    u8 err, ptypenumber, ok, i;
	    u32 fd, fstab, mtab;
	    disk_t* root;
	    MOUNT_POINT fs_root;
	    DRIVER* drv_root;

	// => recuperer le type de la partitions root dans la table des partitions.

		err = blockGetInf(device,root); // get ptypenumber
		switch(err)
		{
		    case BLOCK_ERR_DISK:
		    case BLOCK_ERR_UNINITIALIZED:
	    	    case BLOCK_ERR_MEMORY:
	    	    case BLOCK_ERR_UNKNOWN_DRIVER:
	    	    case BLOCK_ERR_NO_MEDIA:
	    	    case BLOCK_ERR_OK:
			return -1;
		}   

		for(i=0;i<NB_KNOWN_FS;i++)
		{
		    if(!strcmp(type,known_fs[i])){ok=1;break;}
		}
		if(ok!=1){return VFS_ERR_UNKNOWN_DRIVER;}

		
		//known_fs_drivers[i](device,drv_root);
		
		switch(root->type)
		{
		
#ifdef FLOPPY_DRIVER
 		    case floppy:
			known_fs_drivers[i](device,drv_root);
			break;
#endif

#ifdef IDE_HD_DRIVER
		    case ideHd:
			ptypenumber = root->spec.ideHd.partBIOS;
			if(ptypenumber != i){warning(VFS_WARNING_INVALID_MAGIC_NUMBER);}
			known_fs_drivers[ptypenumber](device,drv_root);
			break;
#endif

#ifdef IDE_CD_DRIVER
		    case ideCd:
			known_fs_drivers[i](device,drv_root);
			break;		    
#endif

		    case nothing:
			return(VFS_ERR_UNKNOWN_DEVICE);
		// la j'ai pas trop d'idees ... un fichier genre image ISO ??? pour un boot virtuel c'est sympa ...
		    default :
			return(VFS_ERR_UNKNOWN_DEVICE);   
		// houpss ...
		}


	    drv_root->std_init(device, drv_root);
	
	    fd = drv_root->std_open("/etc/fstab",O_RDONLY,0666);
	    if(fd<0){warning(VFS_WARNING_NO_FSTAB);}
	    
	    fs_root.driver=drv_root;
	    fs_root.path="/";
	    fs_root.device=device;
	    fs_root.type=type;
	    
	    fs_root.driver->std_init(device, drv_root);

	    fstab = open("/etc/fstab", O_RDONLY, 0666);
	    mtab  = open("/etc/mtab", O_WRONLY|O_CREAT, 0666);
	    parse_fstab(fstab,mtab);
	// => si tout va bien, recuperer le fstab et le verifier ligne a ligne.
	// => pour chaque ligne correcte, monter le volume sur le point de montage indique

    return 0;
}
