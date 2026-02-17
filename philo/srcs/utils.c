/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:20:46 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/17 12:29:12 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

bool	is_num_char(const char c)
{
	return (c >= '0' && c <= '9');
}

// これ実装する！！
bool	is_int_str(const char *s)
{
	int	i;

	if (!s)
		return (false);
	i = 0;
	while (s[i])
	{
		if (is_num_char(s[i]) == false)
			return (false);
		i++;
	}
	if (i == 10)
	{
		
	}
	return (true);
}
