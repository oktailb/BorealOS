/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Fonctions Packet (ATAPI et SCSI)
*/

#include <drivers/block/packet/packet.h>


void packetPreventAllowMediumRemoval(bool allow, packet_t *pckt)
{
  pckt->packetSize = 12;
  pckt->packetBuffer[0] = PACKET_PREVENT_ALLOW_MEDIUM_REMOVAL;
  pckt->packetBuffer[1] = 0;
  pckt->packetBuffer[2] = 0;
  pckt->packetBuffer[3] = 0;
  pckt->packetBuffer[4] = allow==TRUE ? 0 : 3;
  pckt->packetBuffer[5] = 0;
  pckt->packetBuffer[6] = 0;
  pckt->packetBuffer[7] = 0;
  pckt->packetBuffer[8] = 0;
  pckt->packetBuffer[9] = 0;
  pckt->packetBuffer[10] = 0;
  pckt->packetBuffer[11] = 0;
  pckt->responseSize = 0;
}
