#
# Ajoute la configuration
include ../../../make_include/config.mk

#
# Ajoute les variables globales
include $(TOPDIR)/make_include/var.mk

#
# Ajoute les r�les g��ales
include $(TOPDIR)/make_include/rules.mk

BLOCK_O = block/block.o
BLOCK_O += ide/ide.o
BLOCK_O += floppy/floppy.o
BLOCK_O += packet/packet.o
BLOCK_O += packetcd/packetcd.o

all: block.o

block.o:
	$(MAKE) -C block
	$(MAKE) -C ide
	$(MAKE) -C floppy
	$(MAKE) -C packet
	$(MAKE) -C packetcd
	ld -i -dN -o block.o $(BLOCK_O)

depend:
	$(MAKE) -C block	depend
	$(MAKE) -C ide	depend
	$(MAKE) -C floppy	depend
	$(MAKE) -C packet	depend
	$(MAKE) -C packetcd	depend

clean:
	$(MAKE) -C block	clean
	$(MAKE) -C ide	clean
	$(MAKE) -C floppy	clean
	$(MAKE) -C packet	clean
	$(MAKE) -C packetcd	clean
	rm -rf *.o *.a *.bin *.elf


#
# Ajoute les cibles g��iques
#include ../../cibles.mk

