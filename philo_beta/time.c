#include "philo.h"

time_t	get_current_unixtime(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return ((tp.tv_sec * 1000000 + tp.tv_usec) / 1000);
}
