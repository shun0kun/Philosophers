#include "philo_bonus.h"

int	parse_args(int argc, char **argv, t_config *cfg)
{
	int	i;

	if (argc != 5 && argc != 6)
		return (-1);
	i = 1;
	while (i < argc)
		if (is_int_str(argv[i++]) == false)
			return (-1);
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
	if (cfg->num_philos < 1 || cfg->time_to_die < 0
		|| cfg->time_to_eat < 0 || cfg->time_to_sleep < 0
		|| (cfg->option_enabled && cfg->times_to_eat < 0))
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_sem		sem;

	if (parse_args(argc, argv, &cfg) < 0)
	{
		write(STDERR_FILENO, "Error: Invalid input\n", 21);
		return (1);
	}
	if (init_sems(&sem, cfg.num_philos) < 0)
	{
		write(STDERR_FILENO, "Error: Semaphore init failed\n", 29);
		return (1);
	}
	simulation(&cfg, &sem);
	close_sems(&sem, cfg.num_philos);
	free(sem.meal);
	unlink_sems(cfg.num_philos);
	return (0);
}
