#include "philo_bonus.h"

long long	current_unixtime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000 + (long long)tp.tv_usec / 1000);
}

// tunable
void	wait_for(long long dur)
{
	long long	target;
	long long	now;
	long long	remaining;

	target = current_unixtime_ms() + dur;
	while (1)
	{
		now = current_unixtime_ms();
		remaining = target - now;
		if (remaining <= 0)
			break ;
		if (remaining > 10)
			usleep(1000);
		else if (remaining > 2)
			usleep(500);
		else
			usleep(10);
	}
}

long long	time_to_wait_in_the_beginning(int id, t_config *cfg)
{
	if (cfg->num_philos % 2 == 0)
	{
		if (id < cfg->num_philos / 2)
			return (0);
		else if (id >= cfg->num_philos / 2)
			return (cfg->time_to_eat);
	}
	else
	{
		if (id == 0)
			return (0);
		else if (id > 0 && id < cfg->num_philos / 2 + 1)
			return (cfg->time_to_eat);
		else if (id >= cfg->num_philos / 2 + 1)
			return (cfg->time_to_eat * 2);
	}
	return (0);
}

long long	time_to_wait_on_interval(int id, t_config *cfg)
{
	(void)id;
	if (cfg->num_philos % 2 == 0 && cfg->time_to_sleep < cfg->time_to_eat)
		return (cfg->time_to_eat - cfg->time_to_sleep);
	else if (cfg->num_philos % 2 != 0
		&& cfg->time_to_sleep < cfg->time_to_eat * 2)
		return (cfg->time_to_eat * 2 - cfg->time_to_sleep);
	else
		return (0);
}
