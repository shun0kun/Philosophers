#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo		*me;
	t_shared	*shared;
	int			cycle;

	me = ((t_thread_args *)arg)->philo;
	shared = ((t_thread_args *)arg)->shared;
	cycle = 0;
	me->time_last_meal = current_unixtime_ms();
	while (1)
	{
		if (philo_pause_until(me, shared, philo_resume_time(me, shared, cycle++)) < 0)
			break ;
		if (philo_take_forks(me, shared) < 0)
			break ;
		if (philo_eat(me, shared) < 0)
			break ;
		if (philo_sleep(me, shared) < 0)
			break ;
		if (philo_think(me, shared) < 0)
			break ;
	}
	return (NULL);
}
//てかこれwhileの中身一つの関数にできるよね。そのまま合体できる。本質的には、ただ長いから分割しているだけになってる。
