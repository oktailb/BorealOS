#
# Cibles g�n�riques pouvant s'appliquer � tous les repertoires
#


clean:
	rm -f *.o *.bin *.elf *.ss *.a *~ log 

depend: 
	gcc -E $(C_FLAGS) -M *.c $(INCLUDE) > .depend  
