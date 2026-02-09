#include "philo.h"

// Get the current Unix time in milliseconds.
long long	current_unixtime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((long long)tp.tv_sec * 1000 + (long long)tp.tv_usec / 1000);
}

long long	time_stamp(long long time_simulation_start)
{
	return (current_unixtime_ms() - time_simulation_start);
}

// Pause until t[ms] since the start time of simulation [ms].
// death_flagチェックをしてもいいかも
int	philo_pause_until(t_philo *philo, t_shared *shared, long long time)
{
	while (current_unixtime_ms() - shared->time_simulation_start < time)
	{
		if (someone_has_died(&shared->death_flag))
			return (-1);
		if (im_dead(philo))
		{
			death_flag_on(&shared->death_flag);
			print_log(time_stamp(shared->time_simulation_start), philo->id, "died", &shared->mu_write);
			return (-1);
		}
	}
	return (0);
}

// このなまえはどうなのか。厳密性において。
long long	philo_resume_time(t_philo *philo, t_shared *shared, int cycle)
{
	if (shared->number_of_philosophers % 2 == 0 && shared->duration_sleep < shared->duration_eat)
	{
		if (philo->id % 2 == 0)
			return (shared->duration_eat * 2 * cycle);
		if (philo->id % 2 != 0)
			return (shared->duration_eat * (1 + 2 * cycle));
	}
	else if (shared->number_of_philosophers % 2 != 0 && shared->duration_sleep < shared->duration_eat * 2)
	{
		if (philo->id == 1)
			return (shared->duration_eat * 2 * cycle);
		if (philo->id != 1 && philo->id % 2 != 0)
			return (shared->duration_eat * (1 + 3 * cycle));
		if (philo->id % 2 == 0)
			return (shared->duration_eat * (2 + 3 * cycle));
	}
	return (0);// Means "Do not wait".
}
