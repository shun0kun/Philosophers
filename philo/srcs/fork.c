/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:17:40 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 15:56:38 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	take_one_and_wait_die(int id, t_shared *shared, int first)
{
	pthread_mutex_lock(&shared->fork[first].mu);
	shared->fork[first].state = BUSY;
	if (try_mark_action(shared, id, "has taken a fork") < 0)
	{
		pthread_mutex_unlock(&shared->fork[first].mu);
		return (-1);
	}
	pthread_mutex_unlock(&shared->fork[first].mu);
	while (1)
	{
		if (must_stop(shared))
			return (-1);
		usleep(200);
	}
	return (0);
}

int	try_take_both(int id, t_shared *shared, int first, int second)
{
	pthread_mutex_lock(&shared->fork[first].mu);
	pthread_mutex_lock(&shared->fork[second].mu);
	if (shared->fork[first].state == IDLE && shared->fork[second].state == IDLE)
	{
		shared->fork[first].state = BUSY;
		shared->fork[second].state = BUSY;
		if (try_mark_action(shared, id, "has taken a fork") < 0
			|| try_mark_action(shared, id, "has taken a fork") < 0)
		{
			shared->fork[first].state = IDLE;
			shared->fork[second].state = IDLE;
			pthread_mutex_unlock(&shared->fork[first].mu);
			pthread_mutex_unlock(&shared->fork[second].mu);
			return (-2);
		}
		pthread_mutex_unlock(&shared->fork[first].mu);
		pthread_mutex_unlock(&shared->fork[second].mu);
		return (0);
	}
	pthread_mutex_unlock(&shared->fork[first].mu);
	pthread_mutex_unlock(&shared->fork[second].mu);
	return (-1);
}

void	set_fork_order(int id, t_shared *shared, int *first, int *second)
{
	if (id % 2 == 0)
	{
		*first = id;
		*second = (id + 1) % shared->cfg.num_philos;
	}
	else
	{
		*first = (id + 1) % shared->cfg.num_philos;
		*second = id;
	}
}

int	take_forks(int id, t_shared *shared)
{
	int	first;
	int	second;
	int	ret;

	set_fork_order(id, shared, &first, &second);
	if (first == second)
		return (take_one_and_wait_die(id, shared, first));
	while (1)
	{
		if (must_stop(shared))
			return (-1);
		ret = try_take_both(id, shared, first, second);
		if (ret == 0)
			break ;
		else if (ret == -2)
			return (-1);
		else
			usleep(500);
	}
	return (0);
}

void	put_forks(int id, t_shared *shared)
{
	int	first;
	int	second;

	set_fork_order(id, shared, &first, &second);
	pthread_mutex_lock(&shared->fork[first].mu);
	pthread_mutex_lock(&shared->fork[second].mu);
	shared->fork[first].state = IDLE;
	shared->fork[second].state = IDLE;
	pthread_mutex_unlock(&shared->fork[first].mu);
	pthread_mutex_unlock(&shared->fork[second].mu);
}
