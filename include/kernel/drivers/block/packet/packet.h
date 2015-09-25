/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Interface Packet (Pour ATAPI et SCSI)
*/

#ifndef __OWL_PACKET_H
#define __OWL_PACKET_H

#ifndef TESTDOS
#include <system.h>
#else
#include "testdos/system.h"
#endif

#define MAX_PACKET_SIZE 16

#define PACKET_PREVENT_ALLOW_MEDIUM_REMOVAL 0x1e

typedef struct __packet
{
  u32 packetSize;                      /* Taille du paquet (en octets) */
  u8 packetBuffer[MAX_PACKET_SIZE];    /* Paquet */
  u32 responseSize;                    /* Taille de la réponse (en octets) */
} packet_t;


/* Prevent or Allow Medium Removal */
void
    packetPreventAllowMediumRemoval(
      bool,                                 /* Autoriser ? */
      packet_t *                            /* Paquet */
    );

#endif
