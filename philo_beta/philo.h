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

typedef struct s_fork
{
	int				owner;
	t_fork_state	state;
	pthread_mutex_t	mu;
}	t_fork;

typedef struct s_death_flag
{
	bool			flag;
	pthread_mutex_t	mu;
}	t_death_flag;

typedef struct s_shared
{
	pthread_mutex_t	mu_write;
	t_fork			*fork;
	t_death_flag	death_flag;
	long long		time_simulation_start;
	bool			someone_died;
	int				number_of_philosophers;
	long long		duration_die;
	long long		duration_eat;
	long long		duration_sleep;
	bool			option_enabled;
}	t_shared;

typedef struct s_philo
{
	int			id;
	t_shared	*shared;
	long long	time_last_meal;
	int			times_i_must_eat;
}	t_philo;

void		*philosopher(void *arg);

int			philo_take_forks(t_philo *philo);
void		philo_put_forks(t_philo *philo);
int			philo_eat(t_philo *philo);
int			philo_sleep(t_philo *philo);		
int			philo_think(t_philo *philo);	

int			pause_until(t_philo *philo, long long time);

long long	current_unixtime_ms(void);
long long	timestamp(long long time_simulation_start);

void		print_log(long long time, int fd, const char *msg, pthread_mutex_t *mu_write);
bool		someone_has_died(t_death_flag *death_flag);
void		death_flag_on(t_death_flag *death_flag);
bool		im_dead(t_philo *philo);

#endif
