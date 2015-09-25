#
# Ajoute la configuration
include ../../../../make_include/config.mk

#
# Ajoute les variables globales
include $(TOPDIR)/make_include/var.mk

#
# Ajoute les r�les g��ales
include $(TOPDIR)/make_include/rules.mk


FICHIER_S=
FICHIER_C=packet.c

all: packet.o


#
# Ajoute les cibles g��iques
include $(TOPDIR)/make_include/cibles.mk


# Doit rester �la fin du fichier tres important
# include a dependency file if one exists
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif
