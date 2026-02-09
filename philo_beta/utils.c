#include "philo.h"

// Memo: Should I lock mutex here ?
void	print_log(long long time, int id, const char *msg, pthread_mutex_t *mu)
{
	pthread_mutex_lock(mu);
	printf("%lld\t%d\t%s\n", time, id, msg);
	pthread_mutex_unlock(mu);
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
