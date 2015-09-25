CC	=	gcc
CFLAGS	=	-W -Wall -pedantic -I$(HOME)/Work/include -g3
SRC	=	insert_ch.c \
		remove_ch.c \
		close_ch.c \
		add_data_ch.c \
		add_first_ch.c \
		add_last_ch.c \
		goto_first_ch.c \
		goto_last_ch.c \
		data_base.c

LIB	=	lib_ch_`uname -m`_`uname -s`.a
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
