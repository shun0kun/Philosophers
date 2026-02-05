#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

typedef enum e_fork_state
{
	IDLE,
	BUSY
}	t_fork_state;

typedef struct s_shared
{
	pthread_mutex_t	mu;
	t_fork_state	*fork;
	long long		start_time;
	int				number_of_philosophers;
	long long		time_to_die;
	long long		time_to_eat;
	long long		time_to_sleep;
	int				option_flag;
}	t_shared;

typedef struct s_philo
{
	int			id;
	t_shared	*shared;
	long long	start_eat_time;
	int			times_i_must_eat;
}	t_philo;

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

//thread unsafe
void	print_log(long long timestamp, int id, const char *msg)
{
	printf("%lld\t%d\t%s\n", timestamp, id, msg);
}


long long	get_current_unixtime()
{
	struct timeval	tp;

	gettimeofday(&tp, NULL);
	return (((long long)tp.tv_sec * 1000000LL + (long long)tp.tv_usec) / 1000LL);
}

int	try_to_take_forks(t_philo *philo)
{
	int	left;
	int	right;

	left = philo->id;
	right = rem(philo->id + 1, philo->shared->number_of_philosophers);
	while (1)
	{
		if (get_current_unixtime() - philo->start_eat_time >= philo->shared->time_to_die)
		{
			pthread_mutex_lock(&philo->shared->mu);
			print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "died");
			pthread_mutex_unlock(&philo->shared->mu);
			return (-1);			
		}
		pthread_mutex_lock(&philo->shared->mu);
		if (left != right && philo->shared->fork[left] == IDLE && philo->shared->fork[right] == IDLE)
		{
			philo->shared->fork[philo->id] = BUSY;
			// print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "has taken a fork");
			philo->shared->fork[rem(philo->id + 1, philo->shared->number_of_philosophers)] = BUSY;
			// print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "has taken a fork");
			pthread_mutex_unlock(&philo->shared->mu);
			return (0);
		}
		pthread_mutex_unlock(&philo->shared->mu);	
	}
}

void	put_forks(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->mu);
	philo->shared->fork[philo->id] = IDLE;
	philo->shared->fork[rem(philo->id + 1, philo->shared->number_of_philosophers)] = IDLE;
	pthread_mutex_unlock(&philo->shared->mu);
}

int	philo_eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->mu);
	print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "is eating");
	pthread_mutex_unlock(&philo->shared->mu);
	philo->start_eat_time = get_current_unixtime();
	while (get_current_unixtime() - philo->start_eat_time < philo->shared->time_to_eat)
	{
		if (get_current_unixtime() - philo->start_eat_time >= philo->shared->time_to_die)
		{
			put_forks(philo);
			pthread_mutex_lock(&philo->shared->mu);
			print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "died");
			pthread_mutex_unlock(&philo->shared->mu);
			return (-1);
		}
	}
	put_forks(philo);
	philo->times_i_must_eat--;
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	long long	start_sleep_time;

	pthread_mutex_lock(&philo->shared->mu);
	print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "is sleeping");
	pthread_mutex_unlock(&philo->shared->mu);
	start_sleep_time = get_current_unixtime();
	while (get_current_unixtime() - start_sleep_time < philo->shared->time_to_sleep)
	{
		if (get_current_unixtime() - philo->start_eat_time >= philo->shared->time_to_die)
		{
			pthread_mutex_lock(&philo->shared->mu);
			print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "died");
			pthread_mutex_unlock(&philo->shared->mu);
			return (-1);			
		}
	}
	return (0);
}

void	philo_think(t_philo *philo)
{
	pthread_mutex_lock(&philo->shared->mu);
	print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "is thinking");
	pthread_mutex_unlock(&philo->shared->mu);
}

int	sleep_starvation(t_philo *philo, long long dt)
{
	long long	t0;

	t0 = get_current_unixtime();
	while (get_current_unixtime() - t0 < dt)
	{
		if (get_current_unixtime() - philo->start_eat_time >= philo->shared->time_to_die)
		{
			pthread_mutex_lock(&philo->shared->mu);
			print_log(get_current_unixtime() - philo->shared->start_time, philo->id, "died");
			pthread_mutex_unlock(&philo->shared->mu);
			return (-1);		
		}
	}
	return (0);
}

int	delay_start(t_philo *philo)
{
	if (philo->shared->number_of_philosophers % 2 == 0)
	{
		if (philo->id % 2 != 0)
			if (sleep_starvation(philo, philo->shared->time_to_eat) < 0)
				return (-1);
	}
	else
	{
		if (philo->id % 2 != 0)
			if (sleep_starvation(philo, philo->shared->time_to_eat) < 0)
				return (-1);
		if (philo->id != 0 && philo->id % 2 == 0)
			if (sleep_starvation(philo, philo->shared->time_to_eat * 2) < 0)
				return (-1);		
	}
	return (0);
}

//めっちゃシンプルになったwwwwwwwwwwwwwwww
void	*philosopher(void *arg)
{
	t_philo	*me;

	me = (t_philo *)arg;
	me->start_eat_time = get_current_unixtime();//"start of the simulation."
	if (delay_start(me) < 0)
		return (NULL);
	while (1)//順番管理機能を実装するのも考えてみる。
	{
		if (try_to_take_forks(me) < 0)
			break ;
		if (philo_eat(me) < 0)
			break ;
		if (me->shared->option_flag && me->times_i_must_eat <= 0)
			break ;
		if (philo_sleep(me) < 0)
			break ;
		philo_think(me);
	}
	return (NULL);
}

int	main(int argc, char **argv)
{
	pthread_t	*threads;
	t_philo		*philo;
	t_shared	shared;
	int			i;

	if (argc == 5)
		shared.option_flag = 0;
	else if (argc == 6)
		shared.option_flag = 1;
	else
		return (1);
	shared.number_of_philosophers = atoi(argv[1]);
	shared.time_to_die = atoi(argv[2]);
	shared.time_to_eat = atoi(argv[3]);
	shared.time_to_sleep = atoi(argv[4]);
	threads = malloc(sizeof(pthread_t) * shared.number_of_philosophers);
	if (!threads)
		return (1);
	philo = malloc(sizeof(t_philo) * shared.number_of_philosophers);
	if (!philo)
		return (1);
	shared.fork = malloc(sizeof(t_fork_state) * shared.number_of_philosophers);
	if (!shared.fork)
		return (1);
	i = 0;
	while (i < shared.number_of_philosophers)
		shared.fork[i++] = IDLE;
	pthread_mutex_init(&shared.mu, NULL);
	shared.start_time = get_current_unixtime();
	i = 0;
	while (i < shared.number_of_philosophers)
	{
		philo[i].id = i;
		philo[i].shared = &shared;
		if (shared.option_flag)
			philo[i].times_i_must_eat = atoi(argv[5]);
		pthread_create(&threads[i], NULL, philosopher, &philo[i]);
		i++;
	}
	i = 0;
	while (i < shared.number_of_philosophers)
		pthread_join(threads[i++], NULL);
	pthread_mutex_destroy(&shared.mu);
	free(threads);
	free(philo);
	return (0);
}
