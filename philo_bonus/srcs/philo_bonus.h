/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_bonus.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:34 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 16:01:15 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

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

// All time values are expressed in microseconds (us).
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
	sem_t	*stop;
}	t_sem;

typedef struct s_termination_worker
{
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

typedef struct s_philo
{
	int			id;
	t_config	*cfg;
	t_sem		*sem;
	long long	start_time;
	long long	last_eat_time;
	int			eat_count;
}	t_philo;

// actions.c
void		philo_eat(t_philo *philo);
void		philo_sleep(t_philo *philo);
void		philo_think(t_philo *philo);

// main.c
int			parse_args(int argc, char **argv, t_config *cfg);
int			main(int argc, char **argv);

// mark.c
void		try_mark_eating(t_philo *philo);
void		try_mark_action(t_philo *philo, const char *msg);

// philosopher.c
void		trigger_termination(sem_t *stop);
void		*termination_worker(void *arg);
void		*monitor_starvation(void *arg);
void		create_and_detatch_threads(t_philo *philo,
				t_termination_worker *tw, t_monitor *monitor);
void		philosopher(t_philo philo);

// semaphore.c
bool		sem_open_all_success(t_sem *sem);
int			init_sems(t_sem *sem, int num_philos);
void		close_sems(t_sem *sem);
void		unlink_sems(void);

// simulation.c
void		wait_all_children(void);
t_philo		get_struct_s_philo(int id, t_config *cfg, t_sem *sem,
				long long start_time);
int			simulation(t_config *cfg, t_sem *sem);

// time.c
long long	utom(long long time_ms);
long long	current_unixtime_us(void);

// time2.c
long long	time_to_wait_first(t_philo *philo);
long long	time_to_wait_on_interval(t_philo *philo);
void		wait_forever(void);
void		wait_for(long long dur);

// utils.c
int			ft_atoi(const char *s);
bool		is_num_char(const char c);
bool		safe(const char *s, int is_neg);
bool		is_int_str(const char *s);
int			get_num_len(int nb);

#endif
