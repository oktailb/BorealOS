#
# Ajoute la configuration
include ../include/make/config.mk

#
# Ajoute les variables globales
include $(TOPDIR)/include/make/var.mk

MBR_TOOL = install_mbr

ALL = $(MBR_TOOL)

all: $(MBR_TOOL)

$(MBR_TOOL):
	$(CC) $(MBR_TOOL).c -o $(MBR_TOOL) -W -Wall -pedantic -O3
clean:
	rm -rf $(ALL)
	rm -rf *.o *.a *~