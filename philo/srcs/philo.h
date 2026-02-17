/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:23 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 12:28:57 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <pthread.h>
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

typedef enum e_fork_state
{
	IDLE,
	BUSY,
}	t_fork_state;

typedef struct s_fork
{
	t_fork_state	state;
	pthread_mutex_t	mu;
}	t_fork;

typedef struct s_stop_flag
{
	bool			flag;
	pthread_mutex_t	mu;
}	t_stop_flag;

typedef struct s_shared
{
	t_config			cfg;
	t_fork				*fork;
	t_stop_flag			stop_flag;
	pthread_mutex_t		write_mu;
	long long			*last_eat_time;
	long long			start_time;
}	t_shared;

typedef struct s_philo
{
	int			id;
	t_shared	*shared;
}	t_philo;

// actions.c
int			philo_eat(int id, t_shared *shared, int *eat_count);
int			philo_sleep(int id, t_shared *shared);
int			philo_think(int id, t_shared *shared);

// characters.c
int			reap_if_dead(int id, t_shared *shared);
void		*reaper(void *arg);
void		*philosopher(void *arg);

// fork.c
int			take_one_and_wait_die(int id, t_shared *shared, int first);
int			try_take_both(int id, t_shared *shared, int first, int second);
int			take_forks(int id, t_shared *shared);
void		put_forks(int id, t_shared *shared);

// main.c
int			parse_args(int argc, char **argv, t_config *cfg);

// mark.c
int			try_mark_eating(int id, t_shared *shared);
int			try_mark_action(t_shared *shared, int id, const char *msg);

// shared.c
int			init_shared(t_shared *shared, t_config *cfg);
void		destroy_shared(t_shared *shared);

// simulation.c
void		create_threads(pthread_t *thread, t_shared *shared, t_philo *philo);
void		join_threads(pthread_t *thread, t_shared *shared);
int			simulation(t_shared *shared);

// stop.c
bool		must_stop(t_shared *shared);
void		set_stop_flag(t_shared *shared);

// time.c
long long	time_to_wait_first(int id, t_shared *shared);
long long	time_to_wait_on_interval(int id, t_shared *shared);
int			wait_for(t_shared *shared, long long dur);
long long	current_unixtime_ms(void);

// utils.c
int			ft_atoi(const char *s);
bool		is_int_str(const char *s);

#endif
