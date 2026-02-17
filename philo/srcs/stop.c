/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   stop.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/17 12:10:49 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 12:10:50 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

bool	must_stop(t_shared *shared)
{
	pthread_mutex_lock(&shared->stop_flag.mu);
	if (shared->stop_flag.flag == true)
	{
		pthread_mutex_unlock(&shared->stop_flag.mu);
		return (true);
	}
	pthread_mutex_unlock(&shared->stop_flag.mu);
	return (false);
}

void	set_stop_flag(t_shared *shared)
{
	pthread_mutex_lock(&shared->stop_flag.mu);
	shared->stop_flag.flag = true;
	pthread_mutex_unlock(&shared->stop_flag.mu);
}
