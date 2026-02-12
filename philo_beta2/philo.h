#ifndef PHILO_H
# define PHILO_H

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <stdbool.h>

typedef struct s_config
{
	int			number_of_philosophers;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			number_of_times_each_philosopher_must_eat;
	bool		option_enabled;
}	t_config;

typedef enum e_fork_state
{
	IDLE,
	BUSY,
}	t_fork_state;

typedef struct s_fork
{
	t_fork_state	*state;
	pthread_mutex_t	mutex;
}	t_fork;

typedef struct s_last_meal
{
	long long		time;
	pthread_mutex_t	mutex;
}	t_last_meal;

typedef struct s_const
{
	int			num_of_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
}	t_const_val;

typedef struct s_shared
{
	t_fork			*fork;
	t_last_meal		*last_meal;
	pthread_mutex_t	write_mutex;
	long long		time_simulation_start;

}	t_shared;

typedef struct s_thread_arg
{
	int			id;
	t_shared	*shared;
}	t_thread_arg;

void	*philosopher(void *arg);
void	*supervisor(void *arg);


#endif
