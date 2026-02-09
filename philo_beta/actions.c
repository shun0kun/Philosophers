#include "philo.h"

int	take_a_fork(t_fork *fork, int id)
{
	pthread_mutex_lock(&fork->mu);
	if (fork->state == IDLE)
	{
		fork->owner = id;
		fork->state = BUSY;
		// print log here ?
		return (0);
	}
	pthread_mutex_unlock(&fork->mu);
	return (-1);
}

int	put_a_fork(t_fork *fork, int id)
{
	pthread_mutex_lock(&fork->mu);
	if (fork->owner == id)
	{
		fork->owner = -1;
		fork->state = IDLE;
		pthread_mutex_unlock(&fork->mu);
		return (0);
	}
	pthread_mutex_unlock(&fork->mu);
	return (-1);
}

// Memo: check if this function can cause deadlock.
// This part can be included in philo_eat.
int		philo_take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id;
	right = rem(philo->id + 1, philo->shared->number_of_philosophers);
	while (1)
	{
		if (im_dead(philo))
		{
			death_flag_on(&philo->shared->death_flag);
			return (-1);
		}
		if (someone_has_died(&philo->shared->death_flag))
			return (-1);
		if (philo->id % 2 == 0)
		{
			if (take_a_fork(&philo->shared->fork[left], philo->id) < 0 && take_a_fork(&philo->shared->fork[right], philo->id) < 0)
			{
				put_a_fork(&philo->shared->fork[left], philo->id);
				put_a_fork(&philo->shared->fork[right], philo->id);
				continue ;
			}
		}
		else
		{
			if (take_a_fork(&philo->shared->fork[right], philo->id) < 0 && take_a_fork(&philo->shared->fork[left], philo->id) < 0)
			{
				put_a_fork(&philo->shared->fork[right], philo->id);
				put_a_fork(&philo->shared->fork[left], philo->id);
				continue ;
			}
		}
	}
}

int		philo_eat(t_philo *philo)
{
	philo->time_last_meal = current_unixtime_ms();
	print_log(timestamp(philo->shared->time_simulation_start), philo->id, "is eating", &philo->shared->mu_write);
	while (current_unixtime_ms() - philo->time_last_meal <= philo->shared->duration_eat)
	{
		if (someone_has_died(&philo->shared->death_flag))
			return (-1);
		if (current_unixtime_ms() - philo->time_last_meal > philo->shared->duration_die)
		{
			death_flag_on(&philo->shared->death_flag);
			print_log(timestamp(philo->shared->time_simulation_start), philo->id, "died", &philo->shared->mu_write);
			return (-1);
		}
	}
	put_a_fork(&philo->shared->fork[philo->id], philo->id);
	put_a_fork(&philo->shared->fork[rem(philo->id + 1, philo->shared->number_of_philosophers)], philo->id);
	if (philo->shared->option_enabled)
	{
		philo->times_i_must_eat--;
		if (philo->times_i_must_eat <= 0)
			return (-1);
	}
	return (0);
}

int		philo_sleep(t_philo *philo)
{
	long long	time_sleep_start;

	time_sleep_start = current_unixtime_ms();
	print_log(timestamp(philo->shared->time_simulation_start), philo->id, "is sleeping", &philo->shared->mu_write);
	while (current_unixtime_ms() - time_sleep_start <= philo->shared->duration_sleep)
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

// Memo: Do I need starvation check here? It seems to be very close.
int		philo_think(t_philo *philo)
{
	if (someone_has_died(&philo->shared->death_flag))
		return (-1);
	print_log(timestamp(philo->shared->time_simulation_start), philo->id, "is thinking", &philo->shared->mu_write);
	return (0);
}
