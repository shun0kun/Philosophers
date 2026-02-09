#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo	*me;
	int		cycle;

	me = (t_philo *)arg;
	cycle = 0;
	me->time_last_meal = current_unixtime_ms();
	while (1)
	{
		if (pause_until(me, adjustment(me, cycle++)) < 0)
			break ;
		if (philo_take_forks(me) < 0)
			break ;
		if (philo_eat(me) < 0)
			break ;
		if (philo_sleep(me) < 0)
			break ;
		if (philo_think(me) < 0)
			break ;
	}
	return (NULL);
}
