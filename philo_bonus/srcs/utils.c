/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:59 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 15:59:49 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

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

bool	safe(const char *s, int is_neg)
{
	static char	*int_limit[] = {"2147483647", "2147483648"};
	int			i;
	char		diff;

	i = 0;
	while (int_limit[is_neg][i])
	{
		diff = s[i] - int_limit[is_neg][i];
		if (diff < 0)
			return (true);
		if (diff > 0)
			return (false);
		i++;
	}
	return (true);
}

bool	is_int_str(const char *s)
{
	int		i;
	int		is_neg;

	if (!s || s[0] == '\0')
		return (false);
	if (s[0] == '0' && s[1] != '\0')
		return (false);
	if (s[0] == '-' && (s[1] == '0' || s[1] == '\0'))
		return (false);
	is_neg = 0;
	if (s[0] == '-')
	{
		s = s + 1;
		is_neg = 1;
	}
	i = 0;
	while (s[i])
		if (is_num_char(s[i++]) == false)
			return (false);
	if (i > 10)
		return (false);
	if (i == 10 && safe(s, is_neg) == false)
		return (false);
	return (true);
}

int	get_num_len(int nb)
{
	int	count;

	if (nb == 0)
		return (1);
	count = 0;
	while (nb > 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}
