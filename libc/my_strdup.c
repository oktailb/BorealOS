#include <my.h>

char	*my_strdup(char *str)
{
	char	*res;
	int	size;
	
	size = my_strlen(str) + 1;
	res = xmalloc(sizeof(*str) * size);
	my_strcpy(res, str);
	return (res);
}
