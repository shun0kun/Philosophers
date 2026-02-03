#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_shared
{
	pthread_mutex_t	*mu;
	int				number_of_people;
	long long		time_to_eat;
	long long		start_time;
}	t_shared;

typedef struct s_psn
{
	int			id;
	int			eat_count;
	t_shared	*shared;
}	t_psn;

void	print_log(int id, const char *msg, long long start_time)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	printf("%lld\t%d\t%s\n", ((long long)tp.tv_sec * 1000000LL + (long long)tp.tv_usec) / 1000LL - start_time, id, msg);
}

void	*person(void *arg)
{
	t_psn	*me;

	me = (t_psn *)arg;
	if (me->id % 2 == 0)
		usleep(me->shared->time_to_eat * 1000);
	while (me->eat_count > 0)
	{
		print_log(me->id, "is thinking", me->shared->start_time);
		pthread_mutex_lock(me->shared->mu);
		print_log(me->id, "is eating", me->shared->start_time);
		usleep(me->shared->time_to_eat * 1000LL);
		me->eat_count--;
		pthread_mutex_unlock(me->shared->mu);
	}
	return (NULL);
}

// ./program numberofpeople timetoeat eatcount
int	main(int argc, char **argv)
{
	pthread_t		*threads;
	pthread_mutex_t	mu;
	t_psn			*psn;
	t_shared		shared;
	struct timeval	tp;

	if (argc != 4)
	{
		dprintf(STDERR_FILENO, "Input error\n");
		return (1);
	}
	shared.number_of_people = atoi(argv[1]);
	shared.time_to_eat = atoi(argv[2]);
	shared.mu = &mu;
	threads = malloc(sizeof(pthread_t) * shared.number_of_people);
	if (!threads)
		return (1);
	psn = malloc(sizeof(t_psn) * shared.number_of_people);
	if (!psn)
		return (1);
	pthread_mutex_init(&mu, NULL);
	gettimeofday(&tp, NULL);
	shared.start_time = ((long long)tp.tv_sec * 1000000LL + (long long)tp.tv_usec) / 1000LL;
	for (int i = 0; i < shared.number_of_people; i++)
	{
		psn[i].id = i;
		psn[i].eat_count = atoi(argv[3]);
		psn[i].shared = &shared;
		pthread_create(&threads[i], NULL, person, &psn[i]);
	}
	for (int i = 0; i < shared.number_of_people; i++)
		pthread_join(threads[i], NULL);
	pthread_mutex_destroy(&mu);
	free(threads);
	free(psn);
	return (0);
}
