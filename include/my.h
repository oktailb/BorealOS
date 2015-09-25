#ifndef __MY_H__
#define __MY_H__

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef union test
{
  int	a;
  char	b[4];
}	BIG;

typedef union stest
{
  short int	a;
  char		b[2];
}	SBIG;

int	my_strlen(char *str);
int	my_putchar(char c);
int	my_putstr(char *str);
char	*my_strdup(char *str);
int	my_strcmp(char *str1, char *str2);
int	my_strncmp(char *str1, char *str2, int n);
char	*my_strcpy(char *dest, char *src);
char	*my_strncpy(char *dest, char *src, int n);
int	my_putnbr_base(int nbr, char *base);
int	my_getnbr_base(char *nbr, char* base);
int	my_putnbr(int nbr);
int	my_putnbr_hexa(int nbr);
int	my_putnbr_HEXA(int nbr);
int	my_getnbr(char *nbr);
char	*my_strstr(char* grande, char *petite);
int	my_is_alphanum(char *str);
int	my_is_alpha(char *str);
int	my_is_num(char *str);
char	*my_strndup(char *str, int n);
int	is_big_endian();
int	to_big_endian(unsigned int var);
int	to_little_endian(unsigned int var);
int	big_to_machine(unsigned int var);
int	little_to_machine(unsigned int var);
void	my_printf(char *str, ...);
int	my_showstr(char *str);
void	*xmalloc(int size);
int	xopen(char *fname, int mode);
int	xread(int fd, void *buff, int size);
int	get_file_size(int fd);
int	my_memcpy(register char *dest, register char *src, register int size);

#endif /* __MY_H__ */
