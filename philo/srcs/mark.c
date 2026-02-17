/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:10:44 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 15:13:46 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	try_mark_eating(int id, t_shared *shared)
{
	long long	tmp;

	pthread_mutex_lock(&shared->write_mu);
	if (must_stop(shared))
	{
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	tmp = shared->eat_stat[id].last_eat_time;
	shared->eat_stat[id].last_eat_time = current_unixtime_ms();
	if (shared->eat_stat[id].last_eat_time - tmp > shared->cfg.time_to_die)
	{
		set_stop_flag(shared);
		printf("%lld\t%d\t%s\n", shared->eat_stat[id].last_eat_time
			- shared->start_time, id + 1, "died");
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	printf("%lld\t%d\t%s\n",
		shared->eat_stat[id].last_eat_time
		- shared->start_time, id + 1, "is eating");
	pthread_mutex_unlock(&shared->write_mu);
	return (0);
}

int	try_mark_action(t_shared *shared, int id, const char *msg)
{
	long long	now;

	pthread_mutex_lock(&shared->write_mu);
	if (must_stop(shared))
	{
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	now = current_unixtime_ms();
	if (now - shared->eat_stat[id].last_eat_time > shared->cfg.time_to_die)
	{
		set_stop_flag(shared);
		printf("%lld\t%d\t%s\n", now - shared->start_time, id + 1, "died");
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	printf("%lld\t%d\t%s\n", now - shared->start_time, id + 1, msg);
	pthread_mutex_unlock(&shared->write_mu);
	return (0);
}
