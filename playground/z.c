#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

int	main(int argc, char **argv)
{
	struct timeval	tp;
	int				f;
	int				l;
	int				time_to_wait;
	
	if (argc != 2)
		return (1);
	time_to_wait = atoi(argv[1]);
	gettimeofday(&tp, NULL);
	printf("%ld,%ld\n", tp.tv_sec, tp.tv_usec);
	f = tp.tv_usec / 1000;
	usleep(time_to_wait * 1000);
	gettimeofday(&tp, NULL);
	l = tp.tv_usec / 1000;
	printf("f=%d, l=%d, Δt=%d\n", f, l, l - f);
	return (0);
}
