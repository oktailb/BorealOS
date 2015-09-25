CC=gcc
CFLAG=-Wall -g
IFLAG= -I. -I./include
PROG_NAME=biblio.exe
PROG_SRC=biblio.c
LIB_SRC=tkgui\tkgui.c
LIB=tkgui.o -lalleg

all: clean $(PROG_NAME) run edit

$(PROG_NAME):
	$(CC) -c $(IFLAG) $(LIB_SRC) $(CFLAG)
	$(CC) $(IFLAG) $(PROG_SRC) -o $(PROG_NAME) $(CFLAG) $(LIB)

clean:
	rm *.o 
	rm $(PROG_NAME)
	
run:
	$(PROG_NAME)
	
edit:
	cd c:\biblio
	rhide
	
      
     



