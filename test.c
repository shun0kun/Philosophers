#include <pthread.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

pthread_mutex_t	mtx;

void	*thread(void *arg)
{
	int	*p_sec = (int *)arg;

	for (int i = 0; i < 10000; i++)
	{
		pthread_mutex_lock(&mtx);
		(*p_sec)++;
		pthread_mutex_unlock(&mtx);
	}
	return (NULL);
}

int	main(void)
{
	pthread_t	p1;
	pthread_t	p2;
	int	cnt;

	cnt = 0;
	pthread_mutex_init(&mtx, NULL);
	pthread_create(&p1, NULL, thread, &cnt);
	pthread_create(&p2, NULL, thread, &cnt);
	pthread_join(p1, NULL);
	pthread_join(p2, NULL);
	pthread_mutex_destroy(&mtx);
	printf("%d\n", cnt);
	return (0);
}
