#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <semaphore.h>
# include <pthread.h>
# include <signal.h>
# include <fcntl.h>
# include <sys/time.h>
# include <stdbool.h>

typedef struct s_config
{
	int			num_philos;
	long long	time_to_die;
	long long	time_to_eat;
	long long	time_to_sleep;
	int			times_to_eat;
	bool		option_enabled;
}	t_config;

typedef struct s_sem
{
	sem_t	*forks;
	sem_t	*write_lock;
	sem_t	*stop;
}	t_sem;


#endif