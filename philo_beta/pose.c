#include "philo.h"

int	pose_untill(t_philo *philo, time_t time)
{
	while (get_current_unixtime() - philo->shared->time_start_simulation < time)
	{
		if (get_current_unixtime() - philo->time_last_meal > philo->shared->duration_die)
			return (-1);
	}
	return (0);
}

int		pose_start(t_philo *philo)
{
	if (philo->shared->number_of_philosophers % 2 == 0)
		if (philo->id % 2 == 0)
			if (pose_untill(philo, philo->shared->duration_eat) < 0)
				return (-1);
	else if (philo->shared->number_of_philosophers % 2 != 0)
	{
		if (philo->id % 2 == 0)
			if (pose_until(philo, philo->shared->duration_eat) < 0)
				return (-1);
		else if (philo->id != 1 && philo->id % 2 != 0)
			if (pose_until(philo, philo->shared->duration_eat * 2) < 0)
				return (-1);
	}
	return (0);
}

int		pose_interval(t_philo *philo)
{

}
