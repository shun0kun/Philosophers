/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:03:10 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 12:07:40 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	philo_eat(int id, t_shared *shared, int *eat_count)
{
	int			ret;

	if (take_forks(id, shared) < 0)
		return (-1);
	if (try_mark_eating(id, shared) < 0)
	{
		put_forks(id, shared);
		return (-1);
	}
	ret = wait_for(shared, shared->cfg.time_to_eat);
	put_forks(id, shared);
	if (ret == 0)
	{
		(*eat_count)++;
		if (shared->cfg.option_enabled && *eat_count
			>= shared->cfg.times_to_eat)
			return (-2);
	}
	return (ret);
}

int	philo_sleep(int id, t_shared *shared)
{
	if (try_mark_action(shared, id, "is sleeping") < 0)
		return (-1);
	return (wait_for(shared, shared->cfg.time_to_sleep));
}

int	philo_think(int id, t_shared *shared)
{
	return (try_mark_action(shared, id, "is thinking"));
}
