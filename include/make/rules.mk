#regles generiques, elles n'influencent pas une regle spécifique
.SUFFIXES: .S .c .o .asm .bin
.S.o:; $(CC) -x assembler-with-cpp -c -o $@ $*.S $(C_FLAGS)
.asm.bin:; $(NASM) -f bin -o $@ $*.asm
.asm.o:; $(NASM)  -f elf -o $@ $*.asm

.c.o:; echo $* ; $(CC) -c -o $@ $*.c $(C_FLAGS) > /dev/null
