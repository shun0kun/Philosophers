/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:46 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/14 12:18:29 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	print_log(t_shared *shared, int id, const char *msg)
{
	pthread_mutex_lock(&shared->write_mu);
	if (shared->stop_flag.flag == true)
	{
		pthread_mutex_unlock(&shared->write_mu);
		return (-1);
	}
	printf("%lld\t%d\t%s\n", time_stamp(shared->start_time), id + 1, msg);
	pthread_mutex_unlock(&shared->write_mu);
	return (0);
}

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

int	ft_atoi(const char *s)
{
	int	sign;
	int	nb;

	sign = 1;
	nb = 0;
	while (*s == ' ' || ((*s >= 9) && (*s <= 13)))
		s++;
	if (*s == '+' || *s == '-')
	{
		if (*s == '-')
			sign = -1;
		s++;
	}
	while (*s >= '0' && *s <= '9')
	{
		nb = nb * 10 + (*s - '0');
		s++;
	}
	return (nb * sign);
}
