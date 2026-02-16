#include "philo_bonus.h"

bool	is_num_char(const char c)
{
	return (c >= '0' && c <= '9');
}

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
	int	nb;

	return (nb);
}

void	trigger_termination(sem_t *stop)
{
	sem_post(stop);
}
