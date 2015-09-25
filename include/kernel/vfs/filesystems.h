#ifndef _FS_LIST_
#define _FS_LIST_

#include "drivers/fs/fat/fat.h"
#include "drivers/fs/iso9660/Iso9660.h"

u8 (*known_fs_drivers[256])(u16 device, DRIVER* drv);
/* a finir ! */

#endif
