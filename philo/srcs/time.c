/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:42 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 12:10:16 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long long	time_to_wait_first(int id, t_shared *shared)
{
	if (shared->cfg.num_philos % 2 == 0)
	{
		if (id % 2 != 0)
			return (shared->cfg.time_to_eat);
		else
			return (0);
	}
	else
	{
		if (id == 0)
			return (0);
		else if (id % 2 != 0)
			return (shared->cfg.time_to_eat);
		else
			return (shared->cfg.time_to_eat * 2);
	}
}

long long	time_to_wait_on_interval(int id, t_shared *shared)
{
	(void)id;
	if (shared->cfg.num_philos % 2 == 0
		&& shared->cfg.time_to_sleep < shared->cfg.time_to_eat)
		return (shared->cfg.time_to_eat - shared->cfg.time_to_sleep);
	else if (shared->cfg.num_philos % 2 != 0
		&& shared->cfg.time_to_sleep < shared->cfg.time_to_eat * 2)
		return (shared->cfg.time_to_eat * 2 - shared->cfg.time_to_sleep);
	else
		return (0);
}

int	wait_for(t_shared *shared, long long dur)
{
	long long	target;
	long long	now;
	long long	remaining;

	target = current_unixtime_ms() + dur;
	while (1)
	{
		now = current_unixtime_ms();
		remaining = target - now;
		if (must_stop(shared))
			return (-1);
		if (remaining <= 0)
			break ;
		if (remaining > 10)
			usleep(1000);
		else if (remaining > 2)
			usleep(500);
		else
			usleep(0);
	}
	return (0);
}

long long	current_unixtime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000 + (long long)tp.tv_usec / 1000);
}
