#include <my.h>

char	*my_strndup(char *str, int n)
{
	char	*res;
	
	res = malloc(sizeof(*str) * n);
	my_strncpy(res, str, n);
	return (res);
}
