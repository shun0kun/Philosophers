#include "philo.h"

int	philo_take_a_fork(t_philo *philo, t_shared *shared, int fork_number)
{
	pthread_mutex_lock(&shared->fork[fork_number].mu);
	if (shared->fork[fork_number].state == IDLE)
	{
		shared->fork[fork_number].owner = philo->id;
		shared->fork[fork_number].state = BUSY;
		print_log(timestamp(shared->time_simulation_start), philo->id, "has taken a fork", &shared->mu_write);
		return (0);
	}
	pthread_mutex_unlock(&shared->fork[fork_number].mu);
	return (-1);
}

int	philo_put_a_fork(t_fork *fork, int id)
{
	pthread_mutex_lock(&fork->mu);
	if (fork->owner == id) //これは哲学者間で意思伝達していることになる？
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
int		philo_take_forks(t_philo *philo, t_shared *shared)
{
	int	left;
	int	right;

	left = philo->id;
	right = rem(philo->id + 1, shared->number_of_philosophers);
	while (1)
	{
		if (someone_has_died(&shared->death_flag))
			return (-1);
		if (im_dead(philo))
		{
			death_flag_on(&shared->death_flag);
			print_log(timestamp(shared->time_simulation_start), philo->id, "died", &shared->mu_write);
			return (-1);
		}
		if (philo->id % 2 == 0)
		{
			if (take_a_fork(&shared->fork[left], philo->id) < 0 || take_a_fork(&shared->fork[right], philo->id) < 0)
			{
				put_a_fork(&shared->fork[left], philo->id);
				put_a_fork(&shared->fork[right], philo->id);
				continue ;
			}
			return (0);
		}
		else
		{
			if (take_a_fork(&shared->fork[right], philo->id) < 0 || take_a_fork(&shared->fork[left], philo->id) < 0)
			{
				put_a_fork(&shared->fork[right], philo->id);
				put_a_fork(&shared->fork[left], philo->id);
				continue ;
			}
			return (0);
		}
	}
}

int		philo_eat(t_philo *philo, t_shared *shared)
{
	philo->time_last_meal = current_unixtime_ms();
	print_log(timestamp(shared->time_simulation_start), philo->id, "is eating", &shared->mu_write);
	while (current_unixtime_ms() - philo->time_last_meal <= shared->duration_eat)
	{
		if (someone_has_died(&shared->death_flag))
			return (-1);
		if (current_unixtime_ms() - philo->time_last_meal > shared->duration_die)
		{
			death_flag_on(&shared->death_flag);
			print_log(timestamp(shared->time_simulation_start), philo->id, "died", &shared->mu_write);
			return (-1);
		}
	}
	put_a_fork(&shared->fork[philo->id], philo->id);
	put_a_fork(&shared->fork[rem(philo->id + 1, shared->number_of_philosophers)], philo->id);
	if (shared->option_enabled)
	{
		philo->times_i_must_eat--;
		if (philo->times_i_must_eat <= 0)
			return (-1);
	}
	return (0);
}

int		philo_sleep(t_philo *philo, t_shared *shared)
{
	long long	time_sleep_start;

	time_sleep_start = current_unixtime_ms();
	print_log(time_stamp(shared->time_simulation_start), philo->id, "is sleeping", &shared->mu_write);
	while (current_unixtime_ms() - time_sleep_start <= shared->duration_sleep)
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

// Memo: Do I need starvation check here? It seems to be very close.
int		philo_think(t_philo *philo, t_shared *shared)
{
	if (someone_has_died(&shared->death_flag))
		return (-1);
	if (im_dead(philo))
	{
		death_flag_on(&shared->death_flag);
		print_log(time_stamp(shared->time_simulation_start), philo->id, "died", &shared->mu_write);
		return (-1);
	}
	print_log(time_stamp(shared->time_simulation_start), philo->id, "is thinking", &shared->mu_write);
	return (0);
}
