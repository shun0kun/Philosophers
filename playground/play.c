#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct s_data
{
	pthread_mutex_t	mutex;
	int				turn;
	int				times;
}	t_data;

void	*writer(void *arg)
{
	t_data	*data = (t_data *)arg;

	while (1)
	{
		
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
	data.turn = 1;
	if (data.times = atoi(argv[1]) < 0)
	{
		write(STDERR_FILENO, "Negative number is invalid\n", 27);
		exit(1);		
	}
	pthread_mutex_init(&data.mutex, NULL);
	for (int i = 0; i < 3; i++)
		pthread_create(&thread[i], NULL, writer, (void *)&data);
	for (int i = 0; i < 3; i++)
		pthread_join(thread[3], NULL);
	pthread_mutex_destroy(&data.mutex);
	return (0);
}
