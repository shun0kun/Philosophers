#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>

typedef enum e_fork_state
{
	IDLE,
	BUSY
}	t_fork_state;

typedef struct s_shared
{
	pthread_mutex_t	mu_write;
	pthread_mutex_t	*mu_fork;
	t_fork_state	*fork;
	time_t			time_start_simulation;
	bool			someone_died;
	int				number_of_philosophers;
	time_t			duration_die;
	time_t			duration_eat;
	time_t			duration_sleep;
	bool			option_enabled;
}	t_shared;

typedef struct s_philo
{
	int			id;
	t_shared	*shared;
	time_t		time_last_meal;
	int			times_i_must_eat;
}	t_philo;

void	*philosopher(void *arg);

int		philo_take_forks(t_philo *philo);
void	philo_put_forks(t_philo *philo);
int		philo_eat(t_philo *philo);
int		philo_sleep(t_philo *philo);		
int		philo_think(t_philo *philo);	

int		pose_start(t_philo *philo);
int		pose_interval(t_philo *philo);

time_t	get_current_unixtime(void);

#endif