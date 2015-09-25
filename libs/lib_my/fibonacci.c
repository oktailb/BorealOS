#include <my.h>

char	*get_nb(int n, char *base)
{
  char	*ret;

  ret = xmalloc(2);
  ret[0] = base[n];
  ret[1] = '\0';
  return (ret);
}

void	multm(char *a[2][2], char *b[2][2], char *res[2][2], char *base)
{
  char	*var1;
  char	*var2;
    
  var1 = xmalloc(sizeof(*var1));
  var2 = xmalloc(sizeof(*var2));
  var1 = bistro_mult(a[0][0], b[0][0], base);
  var2 = bistro_mult(a[0][1], b[1][0], base);
  res[0][0] = bistro_add(var1, var2, base);
  var1 = bistro_mult(a[0][0], b[0][1], base);
  var2 = bistro_mult(a[0][1], b[1][1], base);
  res[0][1] = bistro_add(var1, var2, base);
  var1 = bistro_mult(a[1][0], b[0][0], base);
  var2 = bistro_mult(a[1][1], b[1][0], base);
  res[1][0] = bistro_add(var1, var2, base);
  var1 = bistro_mult(a[1][0], b[0][1], base);
  var2 = bistro_mult(a[1][1], b[1][1], base);
  res[1][1] = bistro_add(var1, var2, base);
}

void	puiss(char *m[2][2], char *pow, char *base)
{
  char	*tmp[2][2];
  char	*res[2][2];

  res[0][0] = xmalloc(sizeof(*res[0][0]));
  res[0][1] = xmalloc(sizeof(*res[0][1]));
  res[1][0] = xmalloc(sizeof(*res[1][0]));
  res[1][1] = xmalloc(sizeof(*res[1][1]));
  tmp[0][0] = xmalloc(sizeof(*tmp[0][0]));
  tmp[0][1] = xmalloc(sizeof(*tmp[0][1]));
  tmp[1][0] = xmalloc(sizeof(*tmp[1][0]));
  tmp[1][1] = xmalloc(sizeof(*tmp[1][1]));
  if (my_strcmp(pow, get_nb(1, base)) == 0)
    return;
  else
    {
      if (my_strcmp(pow, get_nb(2, base)) == 0)
	{
	  multm(m,m,res, base);
	  m[0][0] = res[0][0];
	  m[0][1] = res[0][1];
	  m[1][0] = res[1][0];
	  m[1][1] = res[1][1];
	  return;
	}
    }
  if ((pow[my_strlen(pow) - 1] & 0x01) == 0)   
    {
      puiss(m, div2(my_strdup(pow), base), base);
      puiss(m, get_nb(2, base), base);
      return;
    }
  else        
    {
      tmp[0][0] = m[0][0];
      tmp[0][1] = m[0][1];
      tmp[1][0] = m[1][0];
      tmp[1][1] = m[1][1];
      puiss(m, div2(bistro_sub(pow, get_nb(1, base), base), base), base);
      puiss(m, get_nb(2, base), base);
      multm(m, tmp, res, base);
      m[0][0] = res[0][0];
      m[0][1] = res[0][1];
      m[1][0] = res[1][0];
      m[1][1] = res[1][1];
      return;
    }
}

char	*fib(char *w, char *base)
{
  char	*m[2][2];
  char	*var1;
  char	*res[2][2];
  char	*ret;

  res[0][0] = get_nb(0, base);
  res[0][1] = get_nb(0, base);
  res[1][0] = get_nb(0, base);
  res[1][1] = get_nb(0, base);
  m[0][0] = get_nb(0, base);
  m[0][1] = get_nb(0, base);
  m[1][0] = get_nb(0, base);
  m[1][1] = get_nb(0, base);
  var1 = get_nb(0, base);
  if(my_strcmp(w, get_nb(1, base)) == 0)
    ret = w;
  else
    {
      var1 = bistro_add(res[0][0], m[0][0], base);
      var1[0] = base[1];
      var1 = bistro_add(res[0][1], m[0][1], base);
      var1 = bistro_add(res[1][0], m[1][0], base);
      var1 = bistro_add(res[1][1], m[1][1], base);
      puiss(m, bistro_sub(w, get_nb(1, base), base), base);
      m[1][1] = ret;
    }
  return (ret);
}
