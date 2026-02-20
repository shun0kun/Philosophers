/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:10:49 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 09:19:49 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	must_stop(t_shared *shared)
{
	pthread_mutex_lock(&shared->stop.mu);
	if (shared->stop.flag == true)
	{
		pthread_mutex_unlock(&shared->stop.mu);
		return (true);
	}
	pthread_mutex_unlock(&shared->stop.mu);
	return (false);
}

void	set_stop_flag(t_shared *shared)
{
	pthread_mutex_lock(&shared->stop.mu);
	shared->stop.flag = true;
	pthread_mutex_unlock(&shared->stop.mu);
}
