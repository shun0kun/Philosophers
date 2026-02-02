#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <stdlib.h>

int	main(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	printf("%ld[s], %ld[μs]\n", tp.tv_sec, tp.tv_usec);
	usleep(1000000);//1000000で1s!!!
	gettimeofday(&tp, NULL);
	printf("%ld[s], %ld[μs]\n", tp.tv_sec, tp.tv_usec);	
	return (0);
}
