#
# Ajoute la configuration
include ../../make_include/config.mk

#
# Ajoute les variables globales
include $(TOPDIR)/make_include/var.mk

#
# Ajoute les r�les g��ales
include $(TOPDIR)/make_include/rules.mk


FICHIER_S=
FICHIER_C=vfs.c

all: vfs.o

vfs.o: $(FICHIER_S) $(FICHIER_C)
	$(CC) $(C_FLAG) $(INCLUDE) $(FICHIER_C) -c -o vfs.o

#
# Ajoute les cibles g��iques
include $(TOPDIR)/make_include/cibles.mk


# Doit rester �la fin du fichier tres important
# include a dependency file if one exists
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif
