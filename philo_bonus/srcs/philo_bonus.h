#ifndef PHILO_BONUS_H
# define PHILO_BONUS_H

# include <unistd.h>
# include <stdio.h>
# include <stdlib.h>
# include <semaphore.h>
# include <pthread.h>
# include <signal.h>
# include <sys/types.h>
# include <sys/wait.h>
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
	sem_t	*gate;
	sem_t	*write_lock;
	sem_t	**meal;
	sem_t	*stop;
}	t_sem;

typedef struct s_termination_worker
{
	t_config	*cfg;
	t_sem		*sem;
}	t_termination_worker;

typedef struct s_monitor
{
	int			id;
	long long	start_time;
	long long	*last_meal_time;
	t_config	*cfg;
	t_sem		*sem;
}	t_monitor;

// actions.c

// main.c

// philosopher.c
void		*termination_worker(void *arg);
void		*monitor_starvation(void *arg);
void		philosopher(int id, t_config *cfg, t_sem *sem, long long start_time);

// semaphore.c
void		fill_id_to_meal_sem_name(char *dest, int id);
bool		sem_open_all_success(t_sem *sem, int num_philos);
int			init_sems(t_sem *sem, int num_philos);
void		close_sems(t_sem *sem, int num_philos);
void		unlink_sems(int num_philos);

// simulation.c
void		wait_all_children(void);
int			simulation(t_config *cfg, t_sem *sem);

// time.c
long long	current_unixtime_ms(void);
void		wait_for(long long dur);
long long	time_to_wait_first(int id, t_config *cfg);
long long	time_to_wait_on_interval(int id, t_config *cfg);

// utils.c
bool		is_num_char(const char c);
bool		is_int_str(const char *s);
int			ft_atoi(const char *s);
void		trigger_termination(sem_t *stop);
int			get_num_len(int nb);



// debug
void	fp(const char *s); // delete later!
//


#endif
