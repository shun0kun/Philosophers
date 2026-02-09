#include "philo.h"

// Get the current Unix time in milliseconds.
long long	current_unixtime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000 + (long long)tp.tv_usec / 1000);
}

long long	timestamp(long long time_simulation_start)
{
	return (current_unixtime_ms() - time_simulation_start);
}

// Pause until t[ms] since the start time of simulation [ms].
// death_flagチェックをしてもいいかも
int	pause_until(t_philo *philo, long long time)
{
	while (current_unixtime_ms() - philo->shared->time_simulation_start <= time)
	{
		if (someone_has_died(&philo->shared->death_flag))
			return (-1);
		if (im_dead(philo))
		{
			death_flag_on(&philo->shared->death_flag);
			print_log(timestamp(philo->shared->time_simulation_start), philo->id, "died", &philo->shared->mu_write);
			return (-1);
		}
	}
	return (0);
}

long long	adjustment(t_philo *philo, int cycle)
{
	if (philo->shared->number_of_philosophers % 2 == 0 && philo->shared->duration_sleep < philo->shared->duration_eat)
	{
		if (philo->id % 2 == 0)
			return (philo->shared->duration_eat * 2 * cycle);
		if (philo->id % 2 != 0)
			return (philo->shared->duration_eat * (1 + 2 * cycle));
	}
	else if (philo->shared->number_of_philosophers % 2 != 0 && philo->shared->duration_sleep < philo->shared->duration_eat * 2)
	{
		if (philo->id == 1)
			return (philo->shared->duration_eat * 2 * cycle);
		if (philo->id != 1 && philo->id % 2 != 0)
			return (philo->shared->duration_eat * (1 + 3 * cycle));
		if (philo->id % 2 == 0)
			return (philo->shared->duration_eat * (2 + 3 * cycle));
	}
	return (0);// Means "Do not wait".
}
