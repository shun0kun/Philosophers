#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_data
{
	pthread_mutex_t	mutex;
	int				turn;
	int				times;
	int				roll_call;
	int				escape_flag;
}	t_data;

int	rem(int a, int p)
{
	int	r;

	if (p == 0)
		return (0);
	if (p < 0)
		p = -p;
	if (a >= 0)
		return (a % p);
	else
		return (a - p * (a / p - 1));
}

int	next_turn(int turn)
{
	return (rem(turn, 3) + 1);
}

void	*writer(void *arg)
{
	t_data	*data = (t_data *)arg;
	int		No;

	pthread_mutex_lock(&data->mutex);
	No = data->turn;
	printf("No%d is born.\n", No);
	data->turn = next_turn(data->turn);
	data->roll_call++;
	pthread_mutex_unlock(&data->mutex);
	while (data->roll_call != 3)
		usleep(1000);
	printf("E");fflush(stdout);
	while (1)
	{
		printf("l");fflush(stdout);
		pthread_mutex_lock(&data->mutex);
		if (data->turn == 1 || data->escape_flag)
		{
			data->escape_flag = 1;
			break ;
		}
		data->turn = next_turn(data->turn);
		pthread_mutex_unlock(&data->mutex);
	}
	printf("F");
	while (1)
	{
		pthread_mutex_lock(&data->mutex);
		if (data->times == 0)
			break ;
		if (data->turn == No)
			printf("%d", No);
		if (No == 3)
			printf("\n");
		data->times--;
		data->turn = next_turn(data->turn);
		pthread_mutex_unlock(&data->mutex);
		usleep(1000);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t	thread[3];
	t_data		data;

	if (argc != 2)
	{
		write(STDERR_FILENO, "Give one integer argument.\n", 27);
		exit(1);
	}
	if (data.times = atoi(argv[1]) < 0)
	{
		write(STDERR_FILENO, "Negative number is invalid\n", 27);
		exit(1);
	}
	data.turn = 1;
	data.roll_call = 0;
	data.escape_flag = 0;
	pthread_mutex_init(&data.mutex, NULL);
	for (int i = 0; i < 3; i++)
		pthread_create(&thread[i], NULL, writer, (void *)&data);
	for (int i = 0; i < 3; i++)
		pthread_join(thread[i], NULL);
	pthread_mutex_destroy(&data.mutex);
	return (0);
}

//writeでデッドロック起きてる。whileの中にmutexがあるブロックが複数続いてるからかも。
//それによって、次のブロックにはやく入った人がlockして、前の人がループから出られなくてデッドロック。
//次のブロックは前のひとが全員そのプロックに入ってきたことを前提にしてるから。
//遅れてきても機能するようにするか、みんな終わらないと次のブロックに行かないようにしてみよう。