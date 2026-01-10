#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

void	*thread_func(void *arg)
{
	int	*num = (int *)arg;

	*num = 2;
	usleep(1000000);
	printf("%d\n", *num);
	return (NULL);
}

int	main(void)
{
	pthread_t	thread;
	int			val;

	val = 0;
	if (pthread_create(&thread, NULL, thread_func, (void *)&val))
	{
		printf("fail\n");
		return (1);
	}
	usleep(1);
	val = 1;
	usleep(500000);
	printf("%d", val);
	pthread_join(thread, NULL);
	return (0);
}
