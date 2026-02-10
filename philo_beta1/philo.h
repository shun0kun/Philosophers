#ifndef PHILO_H
# define PHILO_H

#include <pthread.h>
#include <stdbool.h>
#include <sys/time.h>
#include <stdio.h>
#include <stdlib.h>

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
	BUSY
}	t_fork_state;

typedef struct s_fork
{
	int				owner; //これによって哲学者間で意思伝達していることになりうる？どんな場合に？
	t_fork_state	state;
	pthread_mutex_t	mu;
}	t_fork;

typedef struct s_death_flag
{
	bool			flag;
	pthread_mutex_t	mu;
}	t_death_flag;

// 共有データ
// 更に分類すべきか考える。
typedef struct s_shared
{
	t_fork			*fork;
	t_death_flag	death_flag;
	pthread_mutex_t	mu_write;
	long long		time_simulation_start;
	int				number_of_philosophers;
	long long		duration_die;
	long long		duration_eat;
	long long		duration_sleep;
	bool			option_enabled;
}	t_shared;

// 個人データ
typedef struct s_philo
{
	int			id;
	long long	time_last_meal;
	int			times_i_must_eat;
}	t_philo;


// スレッドに引数を渡す用の構造体
typedef struct s_thread_args
{
	t_philo		*philo;
	t_shared	*shared;
}	t_thread_args;

void		*philosopher(void *arg);

int			philo_take_forks(t_philo *philo, t_shared *shared);
int			philo_eat(t_philo *philo, t_shared *shared);
int			philo_sleep(t_philo *philo, t_shared *shared);		
int			philo_think(t_philo *philo, t_shared *shared);	

int			philo_pause_until(t_philo *philo, t_shared *shared, long long time);
long long	philo_resume_time(t_philo *philo, t_shared *shared, int cycle);

long long	current_unixtime_ms(void);
long long	time_stamp(long long time_simulation_start);

void		print_log(t_philo *philo, t_shared *shared, const char *msg);
void		print_log_died(t_philo *philo, t_shared *shared);
bool		someone_has_died(t_death_flag *death_flag);
void		death_flag_on(t_death_flag *death_flag);
bool		im_dead(t_philo *philo, t_shared *shared);
int			rem(int a, int b);
int			ft_atoi(const char *nptr);

#endif
