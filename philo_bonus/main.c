#include "philo_bonus.h"

int	parse_args(int argc, char **argv, t_config *cfg)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (-1);
	i = 1;
	while (i < argc)
	{
		if (is_int_str(argv[i]) == false)
			return (-1);
		i++;
	}
	cfg->num_philos = ft_atoi(argv[1]);
	cfg->time_to_die = ft_atoi(argv[2]);
	cfg->time_to_eat = ft_atoi(argv[3]);
	cfg->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
	{
		cfg->times_to_eat = ft_atoi(argv[5]);
		cfg->option_enabled = true;
	}
	else
		cfg->option_enabled = false;
	if (cfg->num_philos < 1 || cfg->time_to_die < 0 || cfg->time_to_eat < 0 || cfg->time_to_sleep < 0 || (cfg->option_enabled && cfg->times_to_eat < 0))
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_sem		sem;

	// parse command line arguments
	if (parse_args(argc, argv, &cfg) < 0)
	{
		write(STDERR_FILENO, "Invalid input\n", 14);
		return (1);
	}
	
	//semaphore initalization
	sem_unlink("/forks");
	sem.forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, cfg.num_philos);
	if (sem.forks == SEM_FAILED)
		return (1);
	sem_unlink("/write_lock");
	sem.write_lock = sem_open("/write_lock", O_CREAT | O_EXCL, 0644, 1);
	if (sem.write_lock == SEM_FAILED)
	{
		sem_close(sem.forks);
		sem_unlink("/forks");
		return (1);
	}
	sem_unlink("/stop");
	sem.stop = sem_open("/stop", O_CREAT | O_EXCL, 0644, 0);
	if (sem.stop == SEM_FAILED)
	{
		sem_close(sem.forks);
		sem_unlink("/forks");
		sem_close(sem.write_lock);
		sem_unlink("/write_lock");
		return (1);
	}

	// simulation
	simulation(&cfg, &sem);

	// semaphore destroy
	sem_close(sem.forks);
	sem_unlink("/forks");
	sem_close(sem.write_lock);
	sem_unlink("/write_lock");
	sem_close(sem.stop);
	sem_unlink("/stop");
	return (0);
}
