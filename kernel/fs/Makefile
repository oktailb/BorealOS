#
# Ajoute la configuration
include ../../../make_include/config.mk

#
# Ajoute les variables globales
include $(TOPDIR)/make_include/var.mk

#
# Ajoute les r�les générales
include $(TOPDIR)/make_include/rules.mk


FICHIER_S=
FICHIER_C=
FICHIER_O = fat/fat.o
#FICHIER_O += iso9660/iso9660.o

all: fs.o

fs.o:
	$(MAKE) -C fat
#	$(MAKE) -C iso9660
	ld -i -dN -o fs.o $(FICHIER_O)

depend:
	$(MAKE) -C fat   	depend
	$(MAKE) -C iso9660 	depend

clean:
	$(MAKE) -C fat   	clean
	$(MAKE) -C iso9660 	clean
	rm -rf *.o

#
# Ajoute les cibles g��iques
#include $(TOPDIR)/make_include/cibles.mk


# Doit rester à la fin du fichier tres important
# include a dependency file if one exists
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif
