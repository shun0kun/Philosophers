/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time2.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/20 11:14:32 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 11:14:33 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	time_to_wait_first(t_philo *philo)
{
	long long	time_unit;

	if (philo->cfg->num_philos == 1)
		return (0);
	if (philo->cfg->num_philos % 2 == 0)
	{
		if (philo->id % 2 != 0)
			return (philo->cfg->time_to_eat);
		else
			return (0);
	}
	else
	{
		time_unit = philo->cfg->time_to_eat / (philo->cfg->num_philos / 2);
		if (philo->id % 2 == 0)
			return (philo->cfg->time_to_eat + (philo->id / 2) * time_unit);
		else
			return ((philo->id / 2) * time_unit);
	}
}

long long	time_to_wait_on_interval(t_philo *philo)
{
	long long	time_unit;

	if (philo->cfg->num_philos == 1)
		return (0);
	time_unit = philo->cfg->time_to_eat / (philo->cfg->num_philos / 2);
	if (philo->cfg->num_philos % 2 == 0
		&& philo->cfg->time_to_sleep < philo->cfg->time_to_eat)
		return (philo->cfg->time_to_eat - philo->cfg->time_to_sleep);
	else if (philo->cfg->num_philos % 2 != 0
		&& philo->cfg->time_to_sleep < philo->cfg->time_to_eat + time_unit)
		return (philo->cfg->time_to_eat
			+ time_unit - philo->cfg->time_to_sleep);
	else
		return (0);
}

void	wait_forever(void)
{
	while (1)
		usleep(10000000);
}

void	wait_for(long long dur)
{
	long long	target;
	long long	now;
	long long	remaining;

	target = current_unixtime_us() + dur;
	while (1)
	{
		now = current_unixtime_us();
		remaining = target - now;
		if (remaining <= 0)
			break ;
		if (remaining > 10000)
			usleep(1000);
		else if (remaining > 2000)
			usleep(500);
		else
			usleep(10);
	}
}
