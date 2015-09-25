/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Fonctions CD-ROM (Pour ATAPI et SCSI)
*/

#ifndef __OWL_PACKET_CD_H
#define __OWL_PACKET_CD_H

#include <drivers/block/packet/packet.h>

#define PACKET_CD_READ_CAPACITY                0x25
#define PACKET_CD_READ                         0xa8

/* Read Capacity */
void
    packetCdReadCapacity(
      packet_t *                            /* Structure pour le paquet */
    );

/* Read */
void
    packetCdRead(
      u32,                                  /* Adresse du secteur */
      u32,                                  /* Nombre de secteurs */
      packet_t *                            /* Structure pour le paquet */
    );

#endif


