#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo	*me;
	int		cycle;

	me = (t_philo *)arg;
	cycle = 1;
	me->time_last_meal = get_current_unixtime();
	if (pose_start(me) < 0)
		return (NULL);
	while (1)
	{
		if (philo_take_forks(me) < 0)
			break ;
		if (philo_eat(me) < 0)
			break ;
		if (philo_sleep(me) < 0)
			break ;
		if (pose_interval(me) < 0)
			break ;
		cycle++;
	}
	return (NULL);
}
