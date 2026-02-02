#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_data	t_data;

typedef struct s_shared
{
	int				range;
	pthread_mutex_t	*mu;
	int				times;
	int				turn;
}	t_shared;

typedef struct s_data
{
	t_shared	*shared;
	int			id;
}	t_data;

//Does not handle overflow
//Not overflow-safe
int	rem(int a, int b)
{
	if (b == 0)
		return (0);
	if (b < 0)
		b = -b;
	if (a < 0)
	{
		if (a % b == 0)
			return (0);
		else
			return (a - b * (a / b - 1));
	}
	return (a % b);
}

int	next_turn(int turn, int range)
{
	return (rem(turn + 1, range));
}

void	*f(void *arg)
{
	t_data	*data;

	data = (t_data *)arg;
	while (1)
	{
		pthread_mutex_lock(data->shared->mu);
		if (data->shared->times <= 0)
		{
			pthread_mutex_unlock(data->shared->mu);
			break ;
		}
		if (data->id == data->shared->turn)
		{
			if (data->id == data->shared->range - 1)
				printf("%d\n", data->id + 1);
			else
				printf("%d,", data->id + 1);
			data->shared->times--;
			data->shared->turn = next_turn(data->shared->turn, data->shared->range);
		}
		pthread_mutex_unlock(data->shared->mu);
		usleep(1000);
	}
	
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t		*threads;
	t_data			*data;
	t_shared		shared;
	pthread_mutex_t	mu;

	if (argc != 3)
	{
		printf("argc=%d: ", argc);
		dprintf(STDERR_FILENO, "Input error\n");
		return (1);
	}
	shared.range = atoi(argv[1]);
	threads = malloc(sizeof(pthread_t) * shared.range);
	if (!threads)
		return (1);
	data = malloc(sizeof(t_data) * shared.range);
	if (!data)
		return (1);
	pthread_mutex_init(&mu, NULL);
	shared.mu = &mu;
	shared.times = atoi(argv[2]);
	shared.turn = 0;
	for (int i = 0; i < shared.range; i++)
	{
		data[i].id = i;
		data[i].shared = &shared;
		pthread_create(&threads[i], NULL, f, &data[i]);
	}
	for (int i = 0; i < shared.range; i++)
		pthread_join(threads[i], NULL);
	pthread_mutex_destroy(&mu);
	return (0);
}
