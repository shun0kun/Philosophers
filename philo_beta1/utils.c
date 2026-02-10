#include "philo.h"

// Memo: Should I lock mutex here ?
void	print_log(t_philo *philo, t_shared *shared, const char *msg)
{
	pthread_mutex_lock(&shared->mu_write);
	if (shared->death_flag.flag == false)
		printf("%lld\t%d\t%s\n", time_stamp(shared->time_simulation_start), philo->id, msg);
	pthread_mutex_unlock(&shared->mu_write);
}

void	print_log_died(t_philo *philo, t_shared *shared)
{
	pthread_mutex_lock(&shared->mu_write);
	pthread_mutex_lock(&shared->death_flag.mu);
	if (shared->death_flag.flag == false)
	{
		printf("%lld\t%d\t%s\n", time_stamp(shared->time_simulation_start), philo->id, "died");
		shared->death_flag.flag = true;
	}
	pthread_mutex_unlock(&shared->death_flag.mu);	
	pthread_mutex_unlock(&shared->mu_write);
}

void	death_flag_on(t_death_flag *death_flag)
{
	pthread_mutex_lock(&death_flag->mu);
	death_flag->flag = 1;
	pthread_mutex_unlock(&death_flag->mu);	
}

bool	someone_has_died(t_death_flag *death_flag)
{
	pthread_mutex_lock(&death_flag->mu);
	if (death_flag->flag == 1)
	{
		pthread_mutex_unlock(&death_flag->mu);
		return (true);
	}
	pthread_mutex_unlock(&death_flag->mu);
	return (false);
}

bool	im_dead(t_philo *philo, t_shared *shared)
{
	return (current_unixtime_ms() - philo->time_last_meal > shared->duration_die);
}

int	rem(int a, int b)
{
	if (b == 0)
		return (0);
	if (b < 0)
		b = -b;
	if (a < 0)
	{
		if (a % b == 0)
			return (0);
		else
			return (a - b * (a / b - 1));
	}
	return (a % b);
}

int	ft_atoi(const char *nptr)
{
	int	sign;
	int	nb;

	sign = 1;
	nb = 0;
	while (*nptr == ' ' || ((*nptr >= 9) && (*nptr <= 13)))
		nptr++;
	if (*nptr == '+' || *nptr == '-')
	{
		if (*nptr == '-')
			sign = -1;
		nptr++;
	}
	while (*nptr >= '0' && *nptr <= '9')
	{
		nb = nb * 10 + (*nptr - '0');
		nptr++;
	}
	return (nb * sign);
}
