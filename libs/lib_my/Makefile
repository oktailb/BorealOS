CC=gcc
CFLAGS=-W -Wall -pedantic -I. -I$(HOME)/Work/include -g3 -ffast-math -lefence
SRC	=  my_strlen.c \
	my_printf.c \
	my_showstr.c \
	my_putstr.c \
	my_putchar.c \
	my_strdup.c \
	my_strcmp.c \
	my_strncmp.c \
	my_strcpy.c \
	my_strncpy.c \
	my_memcpy.c \
	my_putnbr_base.c \
	my_putnbr_float_base.c \
	my_getnbr_base.c \
	my_getnbr_float_base.c \
	my_strstr.c \
	my_is_alphanum.c \
	my_strndup.c \
	xmalloc.c \
	xopen.c \
	xread.c \
	get_file_size.c \
	get_next_line.c \
	is_big_endian.c \
	pow.c \
	get_part.c \
	bistro_util.c \
	bistro_add.c \
	bistro_sub.c \
	bistro_mult.c \
	fibonacci.c \
	my_str_to_wordtab.c \
	my_strcat.c \
	match.c

LIB = lib_my_`uname -m`_`uname -s`.a
OBJS=$(SRC:.c=.o)

all: $(OBJS) $(LIB)

$(LIB):
	ar -r $(LIB) $(OBJS)
	chmod 0640 $(LIB)
	mv ./$(LIB) $(HOME)/Work/lib

test:
	rm -f ./test
	$(CC) test.c -o test $(CFLAGS) -L../../lib -l_my_$(HOSTTYPE)_$(MACHTYPE) -g3 -W -Wall -pedantic -lm

clean:
	rm -f *.o *~ core.*

fclean: clean
	rm -f $(HOME)/Work/lib/$(LIB)

re: fclean all
