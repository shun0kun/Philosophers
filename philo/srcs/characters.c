/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   characters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:36 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 12:27:36 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	reap_if_dead(int id, t_shared *shared)
{
	long long	now;

	pthread_mutex_lock(&shared->write_mu);
	if (must_stop(shared))
	{
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	now = current_unixtime_ms();
	if (now - shared->last_eat_time[id] > shared->cfg.time_to_die)
	{
		set_stop_flag(shared);
		printf("%lld\t%d\t%s\n", now - shared->start_time, id + 1, "died");
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	pthread_mutex_unlock(&shared->write_mu);
	return (0);
}

void	*reaper(void *arg)
{
	t_shared	*shared;
	int			id;

	shared = (t_shared *)arg;
	while (1)
	{
		id = 0;
		while (id < shared->cfg.num_philos)
		{
			if (reap_if_dead(id, shared) < 0)
				return (NULL);
			id++;
		}
		usleep(500);
	}
	return (NULL);
}

void	*philosopher(void *arg)
{
	int			id;
	t_shared	*shared;
	int			eat_count;

	id = ((t_philo *)arg)->id;
	shared = ((t_philo *)arg)->shared;
	eat_count = 0;
	if (wait_for(shared, time_to_wait_first(id, shared)) < 0)
		return (NULL);
	while (1)
	{
		if (philo_eat(id, shared, &eat_count) < 0)
			break ;
		if (philo_sleep(id, shared) < 0)
			break ;
		if (philo_think(id, shared) < 0)
			break ;
		if (wait_for(shared, time_to_wait_on_interval(id, shared)) < 0)
			break ;
	}
	return (NULL);
}
