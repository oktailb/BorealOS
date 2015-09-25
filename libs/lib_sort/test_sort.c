#include <my_sort.h>
#include <my.h>

int	main(int argc, char *argv[])
{
  int	i;
  int	*tab;

  tab = malloc(sizeof(*tab) * argc);
  for (i = 0; i <argc; i++)
    tab[i] = atoi(argv[i]);
  bidirbubble_sort(tab, argc);
  return (0);
}
