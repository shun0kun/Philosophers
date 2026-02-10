#include "philo.h"

int	parse_arg(int argc, char **argv, t_config *config)
{
	if (argc != 5 && argc != 6)
		return (-1);
	config->number_of_philosophers = ft_atoi(argv[1]);
	config->time_to_die = ft_atoi(argv[2]);
	config->time_to_eat = ft_atoi(argv[3]);
	config->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		config->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);
		config->option_enabled = true;
	}
	else
		config->option_enabled = false;
	return (0);
}

t_shared	*create_shared(const t_config *config)
{
	t_shared	*shared;
	int			i;

	shared = malloc(sizeof(t_shared));
	if (!shared)
		return (NULL);
	shared->number_of_philosophers = config->number_of_philosophers;
	shared->duration_die = config->time_to_die;
	shared->duration_eat = config->time_to_eat;
	shared->duration_sleep = config->time_to_sleep;
	shared->option_enabled = config->option_enabled;
	shared->fork = malloc(sizeof(t_fork) * config->number_of_philosophers);
	if (!shared->fork)
	{
		free(shared);
		return (NULL);
	}
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		shared->fork[i].state = IDLE;
		pthread_mutex_init(&shared->fork[i].mu, NULL);
		i++;
	}
	shared->death_flag.flag = false;
	pthread_mutex_init(&shared->death_flag.mu, NULL);
	pthread_mutex_init(&shared->mu_write, NULL);
	return (shared);
}

t_philo	*create_philos(const t_config *config)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * config->number_of_philosophers);
	if (!philo)
		return (NULL);
	i = 0;
	while (i < config->number_of_philosophers)
	{
		philo[i].id = i + 1;
		if (config->option_enabled)
			philo[i].times_i_must_eat = config->number_of_times_each_philosopher_must_eat;
		i++;
	}
	return (philo);
}

void	destroy_shared(t_shared *shared)
{
	int	i;

	if (!shared)
		return ;
	i = 0;
	while (i < shared->number_of_philosophers)
		pthread_mutex_destroy(&shared->fork[i++].mu);
	free(shared->fork);
	pthread_mutex_destroy(&shared->death_flag.mu);
	pthread_mutex_destroy(&shared->mu_write);
	free(shared);
}

void	destroy_philos(t_philo *philo)
{
	free(philo);
}

t_thread_args	*start_simulation(pthread_t *thread, t_shared *shared, t_philo *philo)
{
	int				i;
	t_thread_args	*args;

	args = malloc(sizeof(t_thread_args) * shared->number_of_philosophers);
	if (!args)
		return (NULL);
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		args[i].philo = &philo[i];
		args[i].shared = shared;
		i++;
	}
	shared->time_simulation_start = current_unixtime_ms();
	i = 0;
	while (i < shared->number_of_philosophers)
	{
		pthread_create(&thread[i], NULL, philosopher, &args[i]);
		i++;
	}
	return (args);
}

void	stop_simulation(pthread_t *thread, t_shared *shared, t_thread_args *args)
{
	int	i;

	i = 0;
	while (i < shared->number_of_philosophers)
		pthread_join(thread[i++], NULL);
	free(args);
}

int	main(int argc, char **argv)
{
	t_config		config;
	t_shared		*shared;
	t_philo			*philo;
	pthread_t		*thread;
	t_thread_args	*args;
	
	if (parse_arg(argc, argv, &config) < 0)
		return (1);
	shared = create_shared(&config);
	if (!shared)
		return (1);
	philo = create_philos(&config);
	if (!philo)
	{
		destroy_shared(shared);
		return (1);
	}
	thread = malloc(sizeof(pthread_t) * shared->number_of_philosophers);
	if (!thread)
	{
		destroy_philos(philo);
		destroy_shared(shared);		
		return (1);
	}
	args = start_simulation(thread, shared, philo);
	if (!args)
	{
		destroy_philos(philo);
		destroy_shared(shared);		
		return (1);
	}
	stop_simulation(thread, shared, args);
	destroy_philos(philo);
	destroy_shared(shared);
	return (0);
}
//このthread_argsをどうにかしよう。
