#
# Ajoute la configuration
include ../../../config.mk

#
# Ajoute les variables globales
include ../../../var.mk

#
# Ajoute les régles générales
include ../../../rules.mk


FICHIER_S=
FICHIER_C=ide.o

all: ide.a

ide.a: $(FICHIER_S) $(FICHIER_C)
	rm -rf ide.a
	$(AR) $(AR_FLAGS) ../serverBlock.a $?


#
# Ajoute les cibles génériques
include ../../../cibles.mk


# Doit rester à la fin du fichier tres important
# include a dependency file if one exists
#
ifeq (.depend,$(wildcard .depend))
include .depend
endif
