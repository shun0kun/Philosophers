/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:18 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 09:59:45 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	philo_eat(t_philo *philo)
{
	sem_wait(philo->sem->gate);
	sem_wait(philo->sem->forks);
	try_mark_action(philo, "has taken a fork");
	sem_wait(philo->sem->forks);
	try_mark_action(philo, "has taken a fork");
	try_mark_eating(philo);
	wait_for(philo->cfg->time_to_eat);
	sem_post(philo->sem->forks);
	sem_post(philo->sem->forks);
	sem_post(philo->sem->gate);
	(philo->eat_count)++;
	if (philo->cfg->option_enabled
		&& philo->eat_count >= philo->cfg->times_to_eat)
	{
		close_sems(philo->sem);
		exit(0);
	}
}

void	philo_sleep(t_philo *philo)
{
	try_mark_action(philo, "is sleeping");
	wait_for(philo->cfg->time_to_sleep);
}

void	philo_think(t_philo *philo)
{
	try_mark_action(philo, "is thinking");
}
