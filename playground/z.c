#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

int	main(void)
{
	int	fd;

	fd = open("z", O_RDWR);
	if (fd < 0)
	{
		perror("Open error");
		return (1);
	}
	write(fd, "Helloo", 6);
	close(fd);
	return 0;
}
