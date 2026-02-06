#include "philo.h"

int	main(int argc, char **argv)
{
	pthread_t	*threads;
	t_philo		*philo;
	t_shared	shared;
	int			i;
	
	if (argc == 5)
		shared.option_enabled = false;
	else if (argc == 6)
		shared.option_enabled = true;
	else
		return (1);
	shared.someone_died = 0;
	shared.number_of_philosophers = atoi(argv[1]);
	shared.duration_die = atoi(argv[2]);
	shared.duration_eat = atoi(argv[3]);
	shared.duration_sleep = atoi(argv[4]);
	threads = malloc(sizeof(pthread_t) * (shared.number_of_philosophers + 1));
	if (!threads)
		return (1);
	philo = malloc(sizeof(t_philo) * (shared.number_of_philosophers + 1));
	if (!philo)
		return (1);
	shared.mu_fork = malloc(sizeof(pthread_mutex_t) * (shared.number_of_philosophers + 1));
	if (!shared.mu_fork)
		return (1);
	pthread_mutex_init(&shared.mu_write, NULL);
	i = 1;
	while (i <= shared.number_of_philosophers)
		pthread_mutex_init(&shared.mu_fork[i++], NULL);
	shared.time_start_simulation = get_current_unixtime();
	i = 1;
	while (i <= shared.number_of_philosophers)
	{
		philo[i].id = i;
		philo[i].shared = &shared;
		if (shared.option_enabled)
			philo[i].times_i_must_eat = atoi(argv[5]);
		pthread_create(&threads[i], NULL, philosopher, &philo[i]);
		i++;
	}
	i = 1;
	while (i <= shared.number_of_philosophers)
		pthread_join(threads[i++], NULL);
	pthread_mutex_destroy(&shared.mu_write);
	i = 1;
	while (i <= shared.number_of_philosophers)
		pthread_mutex_destroy(&shared.mu_fork[i]);
	free(threads);
	free(philo);
	free(shared.mu_fork);
	return (0);
}
