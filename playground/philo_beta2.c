#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/time.h>

typedef struct s_shared
{
	pthread_mutex_t	*forks;
	long long		start_time;
	int				number_of_philosophers;
	int				time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				option_flag;
}	t_shared;

typedef struct s_philo
{
	int			id;
	int			times_i_must_eat;
	long long	start_eating_time;
	t_shared	*shared;
}	t_philo;

//Not overflow-safe
int	rem(int a, int b)
{
	if (b == 0)
		return (0);
	if (b < 0)
		b = -b;
	if (a < 0)
	{
		if (a % b == 0)
			return (0);
		else
			return (a - b * (a / b - 1));
	}
	return (a % b);
}

long long	get_current_unixtime_ms(void)
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return (((long long)tp.tv_sec * 1000000LL + (long long)tp.tv_usec) / 1000LL);
}

void	print_log(long long timestamp, int id, const char *msg)
{
	printf("%lld\t%d\t%s\n", timestamp, id, msg);
}

void	take_fork(pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{

}

void	put_forks(pthread_mutex_t *fork1, pthread_mutex_t *fork2)
{
	pthread_mutex_unlock(fork1);
	pthread_mutex_unlock(fork2);
}

int	wait(t_philo *philo)//
{
	if (philo->shared->number_of_philosophers % 2 == 0)
	{
		if (philo->id % 2 != 0)
			usleep(philo->shared->time_to_eat * 1000);
	}
	else
	{
		if (philo->id % 2 != 0)
			usleep(philo->shared->time_to_eat * 1000 * 2);
		else if (philo->id != 0 && philo->id % 2 == 0)
			usleep(philo->shared->time_to_eat * 1000 * 3);
	}
	return (0);
}

int	philo_eat(t_philo *philo)
{
	while (get_current_unixtime_ms() - philo->start_eating_time < philo->shared->time_to_eat)
	{
		if (get_current_unixtime_ms() - philo->start_eating_time >= philo->shared->time_to_die)
			return (-1);
		usleep(100);
	}
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	while (get_current_unixtime_ms() - philo->start_eating_time < philo->shared->time_to_sleep)
	{
		if (get_current_unixtime_ms() - philo->start_eating_time >= philo->shared->time_to_die)
			return (-1);
		usleep(100);
	}
	return (0);
}

int	take_forks(t_philo *me)
{
	while (1)
	{
		if (get_current_unixtime_ms() - me->start_eating_time >= me->shared->time_to_die)
			return (-1);
		pthread_mutex_lock(&me->shared->forks[me->id]);
		if (pthread_mutex_lock(&me->shared->forks[rem(me->id + 1, 3)]) == 0)
			break ;
		pthread_mutex_unlock(&me->shared->forks[me->id]);
		usleep(100);//この少しの停止になんの意味があるのか。デッドロックを防ぐ効果はあるのか。
	}
	return (0);
}

void	*philosopher(void *arg)
{
	t_philo	*me;

	me = (t_philo *)arg;
	me->start_eating_time = get_current_unixtime_ms();
	if (wait(me) < 0)
		return (NULL);
	while (1)
	{

		if (take_forks(me) < 0)
		{
			print_log(get_current_unixtime_ms() - me->shared->start_time, me->id, "has died");
			break ;
		}
		me->start_eating_time = get_current_unixtime_ms();
		print_log(get_current_unixtime_ms() - me->shared->start_time, me->id, "is eating");
		if (philo_eat(me) < 0)
		{
			put_forks(&me->shared->forks[me->id], &me->shared->forks[rem(me->id + 1, 3)]);
			print_log(get_current_unixtime_ms() - me->shared->start_time, me->id, "has died");
			break ;
		}
		me->times_i_must_eat--;
		put_forks(&me->shared->forks[me->id], &me->shared->forks[rem(me->id + 1, 3)]);
		if (me->shared->option_flag && me->times_i_must_eat <= 0)
			break ;
		print_log(get_current_unixtime_ms() - me->shared->start_time, me->id, "is sleeping");
		if (philo_sleep(me) < 0)
		{
			print_log(get_current_unixtime_ms() - me->shared->start_time, me->id, "has died");
			break ;
		}
		print_log(get_current_unixtime_ms() - me->shared->start_time, me->id, "is thinking");
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t		*threads;
	t_philo			*philo;
	t_shared		shared;
	int				i;

	if (argc == 5)
		shared.option_flag = 0;
	else if (argc == 6)
		shared.option_flag = 1;
	else
		return (1);
	shared.number_of_philosophers = ft_atoi(argv[1]);
	shared.time_to_die = ft_atoi(argv[2]);
	shared.time_to_eat = ft_atoi(argv[3]);
	shared.time_to_sleep = ft_atoi(argv[4]);
	threads = malloc(sizeof(pthread_t) * shared.number_of_philosophers);
	if (!threads)
		return (1);
	philo = malloc(sizeof(t_philo) * shared.number_of_philosophers);
	if (!philo)
		return (1);
	shared.forks = malloc(sizeof(pthread_mutex_t) * shared.number_of_philosophers);
	if (!shared.forks)
		return (1);
	i = 0;
	while (i < shared.number_of_philosophers)
		pthread_mutex_init(&shared.forks[i], NULL);
	shared.start_time = get_current_unixtime_ms();
	i = 0;
	while (i < shared.number_of_philosophers)
	{
		philo[i].id = i;
		if (shared.option_flag)
			philo[i].times_i_must_eat = ft_atoi(argv[5]);
		philo[i].shared = &shared;
		pthread_create(&threads[i], NULL, philosopher, &philo[i]);
	}
	i = 0;
	while (i < shared.number_of_philosophers)
		pthread_join(threads[i], NULL);
	i = 0;
	while (i < shared.number_of_philosophers)
		pthread_mutex_destroy(&shared.forks[i]);
	free(threads);
	free(philo);
	free(shared.forks);
	return (0);
}
