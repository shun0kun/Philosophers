#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>

typedef struct s_data
{
	pthread_mutex_t	mutex;
	int				turn;
	int				times;
}	t_data;

void	*f1(void *arg)
{
	t_data	*data = (t_data *)arg;

	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		if (data->times == 0)
		{
			pthread_mutex_unlock(&data->mutex);
			break ;
		}
		if (data->turn == 1)
		{
			printf("1");
			data->times--;
			data->turn = 2;
		}
		pthread_mutex_unlock(&data->mutex);
	}
	return (NULL);
}

void	*f2(void *arg)
{
	t_data	*data = (t_data *)arg;

	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		if (data->times == 0)
		{
			pthread_mutex_unlock(&data->mutex);
			break ;
		}
		if (data->turn == 2)
		{
			printf("2");
			data->times--;
			data->turn = 3;
		}
		pthread_mutex_unlock(&data->mutex);
	}
	return (NULL);
}

void	*f3(void *arg)
{
	t_data	*data = (t_data *)arg;

	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		if (data->times == 0)
		{
			pthread_mutex_unlock(&data->mutex);
			break ;
		}
		if (data->turn == 3)
		{
			printf("3\n");
			data->times--;
			data->turn = 1;
		}
		pthread_mutex_unlock(&data->mutex);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t	thread[3];
	void		*(*f[3])(void *) = {f1, f2, f3};
	t_data		data;

	if (argc != 2)
	{
		write(STDERR_FILENO, "Give one integer argument.\n", 27);
		exit(1);
	}
	data.times = atoi(argv[1]);
	if (data.times < 0)
	{
		write(STDERR_FILENO, "Negative number is invalid\n", 27);
		exit(1);
	}
	data.turn = 1;
	pthread_mutex_init(&data.mutex, NULL);
	for (int i = 0; i < 3; i++)
		pthread_create(&thread[i], NULL, f[i], (void *)&data);
	for (int i = 0; i < 3; i++)
		pthread_join(thread[i], NULL);
	if (atoi(argv[1]) % 3 != 0)
		printf("\n");
	pthread_mutex_destroy(&data.mutex);
}

//このプログラムを一つのfに統一して作ってみよう。