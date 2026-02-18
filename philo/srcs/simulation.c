/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:32 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 15:23:40 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	create_threads(pthread_t *thread, t_shared *shared, t_philo *philo)
{
	int	i;

	pthread_create(&thread[shared->cfg.num_philos], NULL, reaper, shared);
	i = 0;
	while (i < shared->cfg.num_philos)
	{
		philo[i].id = i;
		philo[i].shared = shared;
		pthread_create(&thread[i], NULL, philosopher, &philo[i]);
		i++;
	}
}

void	join_threads(pthread_t *thread, t_shared *shared)
{
	int	i;

	i = 0;
	while (i < shared->cfg.num_philos)
		pthread_join(thread[i++], NULL);
	pthread_join(thread[i], NULL);
}

int	simulation(t_shared *shared)
{
	pthread_t	*thread;
	t_philo		*philo;
	int			i;

	thread = malloc(sizeof(pthread_t) * (shared->cfg.num_philos + 1));
	if (!thread)
		return (-1);
	philo = malloc(sizeof(t_philo) * shared->cfg.num_philos);
	if (!philo)
	{
		free(thread);
		return (-1);
	}
	shared->start_time = current_unixtime_us();
	i = 0;
	while (i < shared->cfg.num_philos)
		shared->eat_stat[i++].last_eat_time = shared->start_time;
	create_threads(thread, shared, philo);
	join_threads(thread, shared);
	free(thread);
	free(philo);
	return (0);
}
