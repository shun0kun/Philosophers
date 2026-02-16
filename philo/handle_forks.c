/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_forks.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:17:40 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/13 16:59:27 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	could_take_both(int id, t_shared *shared, int first, int second)
{
	pthread_mutex_lock(&shared->fork[first].mu);
	pthread_mutex_lock(&shared->fork[second].mu);
	if (shared->fork[first].state == IDLE && shared->fork[second].state == IDLE)
	{
		shared->fork[first].state = BUSY;
		print_log(shared, id, "take a fork");
		shared->fork[second].state = BUSY;
		print_log(shared, id, "take a fork");
		pthread_mutex_unlock(&shared->fork[first].mu);
		pthread_mutex_unlock(&shared->fork[second].mu);
		return (true);
	}
	pthread_mutex_unlock(&shared->fork[first].mu);
	pthread_mutex_unlock(&shared->fork[second].mu);
	return (false);
}

int	take_one_and_wait_die(int id, t_shared *shared, int first)
{
	pthread_mutex_lock(&shared->fork[first].mu);
	shared->fork[first].state = BUSY;
	print_log(shared, id, "take a fork");
	pthread_mutex_unlock(&shared->fork[first].mu);
	while (1)
	{
		if (must_stop(shared))
			return (-1);
		usleep(1000);
	}
	return (0);
}

int	take_forks(int id, t_shared *shared)
{
	int	first;
	int	second;

	if (id % 2 == 0)
	{
		first = id;
		second = (id + 1) % shared->cfg.num_philos;
	}
	else
	{
		first = (id + 1) % shared->cfg.num_philos;
		second = id;
	}
	if (first == second)
		return (take_one_and_wait_die(id, shared, first));
	while (1)
	{
		if (must_stop(shared))
			return (-1);
		if (could_take_both(id, shared, first, second))
			break ;
		usleep(500);
	}
	return (0);
}

void	put_forks(int id, t_shared *shared)
{
	int	first;
	int	second;

	if (id % 2 == 0)
	{
		first = id;
		second = (id + 1) % shared->cfg.num_philos;
	}
	else
	{
		first = (id + 1) % shared->cfg.num_philos;
		second = id;
	}
	pthread_mutex_lock(&shared->fork[first].mu);
	pthread_mutex_lock(&shared->fork[second].mu);
	shared->fork[first].state = IDLE;
	shared->fork[second].state = IDLE;
	pthread_mutex_unlock(&shared->fork[first].mu);
	pthread_mutex_unlock(&shared->fork[second].mu);
}
