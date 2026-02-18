/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:54 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 15:56:59 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

long long	utom(long long time_us)
{
	return (time_us / 1000);
}

long long	current_unixtime_us(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000000 + tp.tv_usec);
}
