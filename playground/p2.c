#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>

typedef enum e_cs_state
{
	BUSY,
	IDLE
}	t_cs_state;

typedef struct s_shared
{
	pthread_mutex_t	*mu;
	t_cs_state		chopsticks;
	int				time_to_eat;
	int				number_of_people;
}	t_shared;

typedef struct s_psn
{
	int			id;
	int			eat_count;
	t_shared	*shared;
}	t_psn;

void	*person(void *arg)
{
	t_psn	*me;

	me = (t_psn *)arg;
	while (me->eat_count > 0)
	{
		pthread_mutex_lock(&me->shared->mu);
		if (me->shared->chopsticks == IDLE)
		{
			me->
		}
		me->eat_count--;
		pthread_mutex_unlock(&me->shared->mu);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t		*threads;
	pthread_mutex_t	mu;
	t_psn			*psn;
	t_shared		shared;

	if (argv != 4)
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
	return (0);
}
