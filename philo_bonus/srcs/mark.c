/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mark.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:27 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 11:13:51 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	try_mark_eating(t_philo *philo)
{
	long long	tmp;

	sem_wait(philo->sem->write_lock);
	tmp = philo->last_eat_time;
	philo->last_eat_time = current_unixtime_us();
	if (philo->last_eat_time - tmp > philo->cfg->time_to_die)
	{
		printf("%lld\t%d\t%s\n",
			utom(philo->last_eat_time - philo->start_time),
			philo->id + 1, "died");
		trigger_termination(philo->sem->stop);
		wait_forever();
	}
	printf("%lld\t%d\t%s\n",
		utom(philo->last_eat_time - philo->start_time),
		philo->id + 1, "is eating");
	sem_post(philo->sem->write_lock);
}

void	try_mark_action(t_philo *philo, const char *msg)
{
	long long	now;

	sem_wait(philo->sem->write_lock);
	now = current_unixtime_us();
	if (now - philo->last_eat_time > philo->cfg->time_to_die)
	{
		printf("%lld\t%d\t%s\n", utom(now - philo->start_time), philo->id + 1,
			"died");
		trigger_termination(philo->sem->stop);
		wait_forever();
	}
	printf("%lld\t%d\t%s\n", utom(now - philo->start_time), philo->id + 1, msg);
	sem_post(philo->sem->write_lock);
}
