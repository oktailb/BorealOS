#
# Cibles génériques pouvant s'appliquer à tous les repertoires
#


clean:
	rm -f *.o *.bin *.elf *.ss *.a *~ log 

depend: 
	gcc -E $(C_FLAGS) -M *.c $(INCLUDE) > .depend  
