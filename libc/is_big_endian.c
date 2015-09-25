#include <my.h>

int	is_big_endian()
{
  BIG t;
  
  t.a = 1;
  return (t.b[3]);
}

int	to_big_endian(unsigned int var)
{
  BIG t;
  char tmp;

  t.a = var;
  tmp = t.b[0];
  t.b[0] = t.b[3];
  t.b[3] = tmp;
  tmp = t.b[1];
  t.b[1] = t.b[2];
  t.b[2] = tmp;
  return (t.a);
}

int	to_little_endian(unsigned int var)
{
  BIG t;
  char tmp;
  
  t.a = var;
  tmp = t.b[0];
  t.b[0] = t.b[3];
  t.b[3] = tmp;
  tmp = t.b[1];
  t.b[1] = t.b[2];
  t.b[2] = tmp;
  return (t.a);
}

int	little_to_machine(unsigned int var)
{
  if (is_big_endian())
    return (to_big_endian(var));
  else
    return (var);
}

int	big_to_machine(unsigned int var)
{
  if (!is_big_endian())
    return (to_little_endian(var));
  else
    return (var);
}
