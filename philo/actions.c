/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:03:10 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/13 16:17:56 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(int id, t_shared *shared)
{
	int	ret;

	if (take_forks(id, shared) < 0)
		return (-1);
	pthread_mutex_lock(&shared->eat_status[id].mu);
	shared->eat_status[id].last_eat_time = current_unixtime_ms();
	pthread_mutex_unlock(&shared->eat_status[id].mu);
	if (print_log(shared, id, "is eating") < 0)
	{
		put_forks(id, shared);
		return (-1);
	}
	ret = wait_for(shared, shared->cfg.time_to_eat);
	put_forks(id, shared);
	if (ret == 0)
	{
		pthread_mutex_lock(&shared->eat_status[id].mu);
		shared->eat_status[id].times_eaten++;
		pthread_mutex_unlock(&shared->eat_status[id].mu);
	}
	return (ret);
}

int	philo_sleep(int id, t_shared *shared)
{
	if (print_log(shared, id, "is sleeping") < 0)
		return (-1);
	return (wait_for(shared, shared->cfg.time_to_sleep));
}

int	philo_think(int id, t_shared *shared)
{
	return (print_log(shared, id, "is thinking"));
}
