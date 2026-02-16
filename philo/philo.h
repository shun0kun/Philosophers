/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:23 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/13 17:10:45 by sshimots         ###   ########.fr       */
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

typedef struct s_eat_status
{
	long long		last_eat_time;
	int				times_eaten;
	pthread_mutex_t	mu;
}	t_eat_status;

typedef struct s_stop_flag
{
	bool			flag;
	pthread_mutex_t	mu;
}	t_stop_flag;

typedef struct s_shared
{
	t_config			cfg;
	t_fork				*fork;
	t_eat_status		*eat_status;
	t_stop_flag			stop_flag;
	pthread_mutex_t		write_mu;
	long long			start_time;
}	t_shared;

typedef struct s_philo
{
	int			id;
	t_shared	*shared;
}	t_philo;

int			philo_eat(int id, t_shared *shared);
int			philo_sleep(int id, t_shared *shared);
int			philo_think(int id, t_shared *shared);

bool		could_take_both(int id, t_shared *shared, int first, int second);
int			take_one_and_wait_die(int id, t_shared *shared, int first);
int			take_forks(int id, t_shared *shared);
void		put_forks(int id, t_shared *shared);

int			parse_args(int argc, char **argv, t_config *cfg);

int			init_shared(t_shared *shared, t_config *cfg);
void		destroy_shared(t_shared *shared);

int			simulation(t_shared *shared);

void		*philosopher(void *arg);
bool		all_finished_eating(t_shared *shared);
void		*supervisor(void *arg);

long long	current_unixtime_ms(void);
long long	time_stamp(long long start_time);
int			wait_until(t_shared *shared, long long dur);
long long	time_to_wait(int id, t_shared *shared);

long long	current_unixtime_ms(void);
long long	time_stamp(long long start_time);
int			wait_for(t_shared *shared, long long dur);
long long	time_to_wait(int id, t_shared *shared);
long long	time_to_wait_start(int id, t_shared *shared);

int			print_log(t_shared *shared, int id, const char *msg);
bool		must_stop(t_shared *shared);
int			ft_atoi(const char *s);

#endif
