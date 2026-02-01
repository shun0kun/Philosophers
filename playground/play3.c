#include <stdio.h>
#include <pthread.h>

void	*f(void *arg)
{
	pthread_mutex_t	*mu;

	mu = (pthread_mutex_t *)arg;
	pthread_mutex_lock(mu);
	printf("a");
	pthread_mutex_unlock(mu);
	return (NULL);
}

int	main(void)
{
	pthread_t	thread;
	pthread_mutex_t	mu;

	pthread_create(&thread, NULL, f, (void *)&mu);
	pthread_join(thread, NULL);
}