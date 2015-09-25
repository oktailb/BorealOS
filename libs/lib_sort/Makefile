CC=gcc
CFLAGS=-W -Wall -pedantic -I$(HOME)/Work/include -O3
SRC= bubble.c insert.c quick.c bidirbubble.c
LIB = lib_sort_`uname -m`_`uname -s`.a
OBJS=$(SRC:.c=.o)

all: $(OBJS) $(LIB)

$(LIB):
	ar -r $(LIB) $(OBJS)
	chmod 0640 $(LIB)
	mv ./$(LIB) $(HOME)/Work/lib

clean:
	rm -rf *.o *~

test:
	$(CC) -o test test_sort.c $(CFLAGS) -L$(HOME)/Work/lib -l_my_$(HOSTTYPE)_$(MACHTYPE).a -l_sort_$(HOSTTYPE)_$(MACHTYPE).a

fclean: clean
	rm -rf $(HOME)/Work/lib/$(LIB)

re: fclean all
