/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   eat_count.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 15:30:11 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 15:44:31 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	is_full(int id, t_shared *shared)
{
	if (shared->cfg.option_enabled == false)
		return (false);
	pthread_mutex_lock(&shared->eat_stat[id].mu);
	if (shared->eat_stat[id].eat_count >= shared->cfg.times_to_eat)
	{
		pthread_mutex_unlock(&shared->eat_stat[id].mu);
		return (true);
	}
	pthread_mutex_unlock(&shared->eat_stat[id].mu);
	return (false);
}

bool	everyone_is_full(t_shared *shared)
{
	int	id;

	if (shared->cfg.option_enabled == false)
		return (false);
	id = 0;
	while (id < shared->cfg.num_philos)
	{
		if (is_full(id, shared) == false)
			return (false);
		id++;
	}
	return (true);
}

void	increment_eat_count(int id, t_shared *shared)
{
	pthread_mutex_lock(&shared->eat_stat[id].mu);
	(shared->eat_stat[id].eat_count)++;
	pthread_mutex_unlock(&shared->eat_stat[id].mu);
}
