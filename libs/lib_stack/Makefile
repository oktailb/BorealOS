CC	=	gcc
CFLAGS	=	-W -Wall -pedantic -I$(HOME)/Work/include -O3
SRC	=	stack.c
LIB	=	lib_stack_`uname -m`_`uname -s`.a
OBJS	=	$(SRC:.c=.o)

all: $(OBJS) $(LIB)

$(LIB):
	ar -r $(LIB) $(OBJS)
	chmod 0640 $(LIB)
	mv ./$(LIB) $(HOME)/Work/lib

clean:
	rm -rf *.o *~

fclean: clean
	rm -rf $(HOME)/Work/lib/$(LIB)

re: fclean all
