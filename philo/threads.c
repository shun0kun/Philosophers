/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   threads.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:36 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/16 09:52:35 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*philosopher(void *arg)
{
	int			id;
	t_shared	*shared;

	id = ((t_philo *)arg)->id;
	shared = ((t_philo *)arg)->shared;
	shared->eat_status[id].times_eaten = 0;
	if (wait_for(shared, time_to_wait_start(id, shared)) < 0)
		return (NULL);
	while (1)
	{
		if (philo_eat(id, shared) < 0)
			break ;
		if (philo_sleep(id, shared) < 0)
			break ;
		if (philo_think(id, shared) < 0)
			break ;
		if (wait_for(shared, time_to_wait(id, shared)) < 0)
			break ;
	}
	return (NULL);
}

bool	all_finished_eating(t_shared *shared)
{
	int		i;

	if (shared->cfg.option_enabled == false)
		return (false);
	i = 0;
	while (i < shared->cfg.num_philos)
	{
		pthread_mutex_lock(&shared->eat_status[i].mu);
		if (shared->eat_status[i].times_eaten < shared->cfg.times_to_eat)
		{
			pthread_mutex_unlock(&shared->eat_status[i].mu);
			return (false);
		}
		pthread_mutex_unlock(&shared->eat_status[i].mu);
		i++;
	}
	return (true);
}

int	handle_death(int id, t_shared *shared)
{
	pthread_mutex_lock(&shared->eat_status[id].mu);
	if (current_unixtime_ms() - shared->eat_status[id].last_eat_time
		> shared->cfg.time_to_die)
	{
		pthread_mutex_lock(&shared->write_mu);
		pthread_mutex_lock(&shared->stop_flag.mu);
		shared->stop_flag.flag = true;
		pthread_mutex_unlock(&shared->stop_flag.mu);
		printf("%lld\t%d\t%s\n",
			time_stamp(shared->start_time), id + 1, "died");
		pthread_mutex_unlock(&shared->write_mu);
		pthread_mutex_unlock(&shared->eat_status[id].mu);
		return (-1);
	}
	pthread_mutex_unlock(&shared->eat_status[id].mu);
	return (0);
}

void	*supervisor(void *arg)
{
	t_shared	*shared;
	int			id;

	shared = (t_shared *)arg;
	while (1)
	{
		id = 0;
		while (id < shared->cfg.num_philos)
		{
			if (handle_death(id, shared) < 0)
				return (NULL);
			id++;
		}
		if (shared->cfg.option_enabled == true && all_finished_eating(shared))
		{
			pthread_mutex_lock(&shared->write_mu);
			pthread_mutex_lock(&shared->stop_flag.mu);
			shared->stop_flag.flag = true;
			pthread_mutex_unlock(&shared->stop_flag.mu);
			pthread_mutex_unlock(&shared->write_mu);
			return (NULL);
		}
		usleep(500);
	}
	return (NULL);
}
