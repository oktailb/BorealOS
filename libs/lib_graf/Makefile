CC	=	gcc
CFLAGS	=	-W -Wall -pedantic -I$(HOME)/Work/include -g3 -L../../lib
SRC	=	parcours.c \
		connexes.c \
		cliques.c \
		coloriage.c \
		gnuplot.c \
		parfait.c \
		afficher.c \
		ajouterSommet.c \
		retirerSommet.c \
		existe_sommet.c \
		init.c \
		initRandK.c \
		taille.c \
		adjacent.c \
		ajouterArc.c \
		retirerArc.c \
		effacer.c \
		initRand.c \
		init_graphe.c \
		init_graphe_from_fd.c \
		grafdup.c \
		compare.c \
		extract.c \
		name_to_number.c

LIB 	= 	lib_graf_`uname -m`_`uname -s`.a
OBJS	=	$(SRC:.c=.o)

all: $(OBJS) $(LIB)

$(LIB):
	ar -r $(LIB) $(OBJS)
	chmod 0640 $(LIB)
	mv ./$(LIB) $(HOME)/Work/lib

clean:
	rm -rf *.o *~

fclean: clean
	rm -f $(HOME)/Work/lib/$(LIB)
	rm -f ./test

re: fclean all

test:
	rm -f ./test 
	$(CC) test_lib.c -o test $(CFLAGS) -l_graf_$(HOSTTYPE)_$(MACHTYPE) -l_my_$(HOSTTYPE)_$(MACHTYPE) -l_sort_$(HOSTTYPE)_$(MACHTYPE) -lm -l_stack_$(HOSTTYPE)_$(MACHTYPE) -l_ch_$(HOSTTYPE)_$(MACHTYPE)
