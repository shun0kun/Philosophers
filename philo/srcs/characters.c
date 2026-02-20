/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   characters.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:36 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 10:18:31 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	reap_if_dead(int id, t_shared *shared)
{
	long long	now;

	if (is_full(id, shared))
		return (0);
	pthread_mutex_lock(&shared->mark_mu);
	if (must_stop(shared))
	{
		pthread_mutex_unlock(&shared->mark_mu);
		return (-1);
	}
	now = current_unixtime_us();
	if (now - shared->eat[id].last_time > shared->cfg.time_to_die)
	{
		set_stop_flag(shared);
		printf("%lld\t%d\t%s\n",
			utom(now - shared->start_time), id + 1, "died");
		pthread_mutex_unlock(&shared->mark_mu);
		return (-1);
	}
	pthread_mutex_unlock(&shared->mark_mu);
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
		if (shared->cfg.option_enabled && everyone_is_full(shared))
			return (NULL);
		usleep(500);
	}
	return (NULL);
}

void	*philosopher(void *arg)
{
	int			id;
	t_shared	*shared;

	id = ((t_philo *)arg)->id;
	shared = ((t_philo *)arg)->shared;
	if (shared->cfg.option_enabled && shared->cfg.times_to_eat == 0)
		return (NULL);
	if (wait_for(shared, time_to_wait_first(id, shared)) < 0)
		return (NULL);
	while (1)
	{
		if (philo_eat(id, shared) < 0)
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
