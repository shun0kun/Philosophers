/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:28 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 10:19:39 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_shared_mutexes(t_shared *shared, t_config *cfg)
{
	int	i;

	i = 0;
	while (i < cfg->num_philos)
		pthread_mutex_init(&shared->fork[i++].mu, NULL);
	i = 0;
	while (i < cfg->num_philos)
		pthread_mutex_init(&shared->eat[i++].mu, NULL);
	pthread_mutex_init(&shared->stop.mu, NULL);
	pthread_mutex_init(&shared->mark_mu, NULL);
}

int	init_shared(t_shared *shared, t_config *cfg)
{
	int	i;

	if (!shared || !cfg)
		return (-1);
	shared->cfg = *cfg;
	shared->fork = malloc(sizeof(t_fork) * cfg->num_philos);
	if (!shared->fork)
		return (-1);
	shared->eat = malloc(sizeof(t_eat) * cfg->num_philos);
	if (!shared->eat)
	{
		free(shared->fork);
		return (-1);
	}
	shared->stop.flag = false;
	i = 0;
	while (i < cfg->num_philos)
		shared->fork[i++].state = IDLE;
	i = 0;
	while (i < cfg->num_philos)
		shared->eat[i++].count = 0;
	init_shared_mutexes(shared, cfg);
	return (0);
}

void	destroy_shared(t_shared *shared)
{
	int	i;

	if (!shared)
		return ;
	i = 0;
	while (i < shared->cfg.num_philos)
		pthread_mutex_destroy(&shared->fork[i++].mu);
	i = 0;
	while (i < shared->cfg.num_philos)
		pthread_mutex_destroy(&shared->eat[i++].mu);
	pthread_mutex_destroy(&shared->stop.mu);
	pthread_mutex_destroy(&shared->mark_mu);
	free(shared->fork);
	free(shared->eat);
}
