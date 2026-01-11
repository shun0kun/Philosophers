#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>

typedef enum e_philo_state
{
	EATING,
	SLEEPING,
	THINKING
}	t_philo_state;

typedef enum e_fork_state
{
	IDLE,
	BUSY
}	t_fork_state;

typedef struct s_philo_ctx
{
	pthread_mutex_t	mtx;
	pthread_t		*threads;
	t_fork_state	*fork_state;
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
}	t_philo_ctx;

#endif
