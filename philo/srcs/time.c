/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:42 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 15:25:01 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	utom(long long time_us)
{
	return (time_us / 1000);
}

long long	time_to_wait_first(int id, t_shared *shared)
{
	long long	time_unit;

	if (shared->cfg.num_philos == 1)
		return (0);
	if (shared->cfg.num_philos % 2 == 0)
	{
		if (id % 2 != 0)
			return (shared->cfg.time_to_eat);
		else
			return (0);
	}
	else
	{
		time_unit = shared->cfg.time_to_eat / (shared->cfg.num_philos / 2);
		if (id % 2 == 0)
			return (shared->cfg.time_to_eat + (id / 2) * time_unit);
		else
			return ((id / 2) * time_unit);
	}
}

long long	time_to_wait_on_interval(int id, t_shared *shared)
{
	long long	time_unit;

	(void)id;
	if (shared->cfg.num_philos == 1)
		return (0);
	time_unit = shared->cfg.time_to_eat / (shared->cfg.num_philos / 2);
	if (shared->cfg.num_philos % 2 == 0
		&& shared->cfg.time_to_sleep < shared->cfg.time_to_eat)
		return (shared->cfg.time_to_eat - shared->cfg.time_to_sleep);
	else if (shared->cfg.num_philos % 2 != 0
		&& shared->cfg.time_to_sleep < shared->cfg.time_to_eat + time_unit)
		return (shared->cfg.time_to_eat
			+ time_unit - shared->cfg.time_to_sleep);
	else
		return (0);
}

int	wait_for(t_shared *shared, long long dur)
{
	long long	target;
	long long	now;
	long long	remaining;

	target = current_unixtime_us() + dur;
	while (1)
	{
		now = current_unixtime_us();
		remaining = target - now;
		if (must_stop(shared))
			return (-1);
		if (remaining <= 0)
			break ;
		if (remaining > 10000)
			usleep(1000);
		else if (remaining > 2000)
			usleep(500);
		else
			usleep(0);
	}
	return (0);
}

long long	current_unixtime_us(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000000 + tp.tv_usec);
}
