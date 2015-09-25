/*
  Projet de programmation OWL
  www.owl-os.fr.st
  Fonctions CD-ROM (Pour ATAPI et SCSI)
*/

#include <drivers/block/packetcd/packetcd.h>

void packetCdReadCapacity(packet_t *pckt)
{
  pckt->packetSize = 12;
  pckt->packetBuffer[0] = PACKET_CD_READ_CAPACITY;
  pckt->packetBuffer[1] = 0;
  pckt->packetBuffer[2] = 0;
  pckt->packetBuffer[3] = 0;
  pckt->packetBuffer[4] = 0;
  pckt->packetBuffer[5] = 0;
  pckt->packetBuffer[6] = 0;
  pckt->packetBuffer[7] = 0;
  pckt->packetBuffer[8] = 0;
  pckt->packetBuffer[9] = 0;
  pckt->packetBuffer[10] = 0;
  pckt->packetBuffer[11] = 0;
  pckt->responseSize = 8;
}

void packetCdRead(u32 lba, u32 count, packet_t *pckt)
{
  pckt->packetSize = 12;
  pckt->packetBuffer[0] = PACKET_CD_READ;
  pckt->packetBuffer[1] = 0;
  pckt->packetBuffer[2] = (lba>>24)&0xff;
  pckt->packetBuffer[3] = (lba>>16)&0xff;
  pckt->packetBuffer[4] = (lba>>8)&0xff;
  pckt->packetBuffer[5] = lba&0xff;
  pckt->packetBuffer[6] = (count>>24)&0xff;
  pckt->packetBuffer[7] = (count>>16)&0xff;
  pckt->packetBuffer[8] = (count>>8)&0xff;
  pckt->packetBuffer[9] = count&0xff;
  pckt->packetBuffer[10] = 0;
  pckt->packetBuffer[11] = 0;
  pckt->responseSize = 2048 * count;
}
