#include <graf.h>

int	name_to_number(t_graphe *graphe, char *name)
{
    t_ch_ch_int		*ttmp;

    for (ttmp = graphe->graphe;ttmp != NULL; ttmp = ttmp->next)
      if (!my_strcmp(ttmp->name, name))
	return (ttmp->number);
    return (0);
}

char	*number_to_name(t_graphe *graphe, int number)
{
    t_ch_ch_int		*ttmp;

    for (ttmp = graphe->graphe;ttmp != NULL; ttmp = ttmp->next)
      if (ttmp->number == number)
	return (ttmp->name);
    return (NULL);
}
