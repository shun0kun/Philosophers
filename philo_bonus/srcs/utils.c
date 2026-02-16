#include "philo_bonus.h"

bool	is_num_char(const char c)
{
	return (c >= '0' && c <= '9');
}

// 空文字も弾く！！！！！
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

void	trigger_termination(sem_t *stop)
{
	sem_post(stop);
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


// debug
void	fp(const char *s)
{
	dprintf(STDERR_FILENO, "%s\n", s);
	fflush(stderr);
}
