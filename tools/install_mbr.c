#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int	main(int argc, char *argv[])
{
  int	dest_fd;
  int	boot_fd;

  int	i;

  unsigned char	*dest_buf;
  unsigned char	*boot_buf;

  if (argc < 3)
    {
      printf("Usage : %s destination_device bootsector_file\n", argv[0]);
      exit(0);
    }
  else
    {
      dest_fd = open(argv[1], O_RDWR, 0640);
      if (dest_fd < 0)
	{
	  perror("open :");
	  exit (-1);
	}
      boot_fd = open(argv[2], O_RDONLY, 0640);
      if (boot_fd < 0)
	{
	  perror("open :");
	  exit (-1);
	}
      dest_buf = malloc(512 * sizeof(*dest_buf));
      boot_buf = malloc(512 * sizeof(*boot_buf));
      read(dest_fd, dest_buf, 512);
      printf ("dest: \n");
      for (i=0; i< 512; i++)
	{
	  printf("%.2X ", dest_buf[i]);
	  if (!((i+1)%16))printf("\n");
	}
      read(boot_fd, boot_buf, 512);
      printf ("boot: \n");
      for (i=0; i< 512; i++)
	{
	  printf("%.2X ", dest_buf[i]);
	  if(!((i+1)%16))printf("\n");
	}
      memcpy(&boot_buf[446], &dest_buf[446], 64);
      lseek(dest_fd, 0, SEEK_SET);
      write(dest_fd, boot_buf, 512);
      free(dest_buf);
      free(boot_buf);
      close(dest_fd);
      close(boot_fd);
    }
  return (0);
}
