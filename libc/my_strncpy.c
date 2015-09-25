#include <my.h>

char	*my_strncpy(char *dest, char *src, int n)
{
	int	i;
	
	i = 0;
	while ((n > 0) && (src[i] != '\0'))
	{
		dest[i] = src[i];
		i++;
		n--;
	}
	if (n != 0)
		dest[i] = '\0';
	return (dest);
}
