#
# Ajoute la configuration
include ./include/make/config.mk

#
# Ajoute les variables globales
include $(TOPDIR)/include/make/var.mk

all: image

image:
	$(MAKE) -C boot
#	$(MAKE) -C kernel
#	$(MAKE) -C libc
	$(MAKE) -C tools

debug: image
	objdump kernel/kernel.elf --disassemble > kernel.disassembly
	nm kernel/kernel.elf --numeric-sort > kernel.syms

install:
	strip kernel.elf
	mount $(FLOPPY)
	cp -f kernel.elf $(FLOPPY)/kernel
	umount $(FLOPPY)

clean:
	$(MAKE) -C boot		clean
#	$(MAKE) -C kernel	clean
#	$(MAKE) -C libc		clean
	$(MAKE) -C tools	clean

fclean: clean
	rm -f *~ kernel.elf kernel.disassembly kernel.syms

re: fclean all

depend:
	$(MAKE) -C boot		depend
#	$(MAKE) -C kernel	depend
#	$(MAKE) -C libc		depend
	$(MAKE) -C tools	depend

tag:
	rm -f TAGS
	etags --lang=c --recurse=yes *.[ch]
	etags --lang=asm --recurse=yes --append *.asm *.s
