/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   shared.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:28 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/16 08:24:58 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	init_shared_mutexex(t_shared *shared, t_config *cfg)
{
	int	i;

	i = 0;
	while (i < cfg->num_philos)
		pthread_mutex_init(&shared->fork[i++].mu, NULL);
	i = 0;
	while (i < cfg->num_philos)
		pthread_mutex_init(&shared->eat_status[i++].mu, NULL);
	i = 0;
	while (i < cfg->num_philos)
		pthread_mutex_init(&shared->eat_status[i++].mu, NULL);
	pthread_mutex_init(&shared->stop_flag.mu, NULL);
	pthread_mutex_init(&shared->write_mu, NULL);
}

int	init_shared(t_shared *shared, t_config *cfg)
{
	int	i;

	shared->cfg = *cfg;
	shared->fork = malloc(sizeof(t_fork) * cfg->num_philos);
	if (!shared->fork)
		return (-1);
	shared->eat_status = malloc(sizeof(t_eat_status) * cfg->num_philos);
	if (!shared->eat_status)
	{
		free(shared->fork);
		return (-1);
	}
	shared->stop_flag.flag = false;
	i = 0;
	while (i < cfg->num_philos)
		shared->fork[i++].state = IDLE;
	init_shared_mutexex(shared, cfg);
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
		pthread_mutex_destroy(&shared->eat_status[i++].mu);
	if (shared->cfg.option_enabled == true)
	{
		i = 0;
		while (i < shared->cfg.num_philos)
			pthread_mutex_destroy(&shared->eat_status[i++].mu);
	}
	pthread_mutex_destroy(&shared->stop_flag.mu);
	pthread_mutex_destroy(&shared->write_mu);
	free(shared->fork);
	free(shared->eat_status);
}
