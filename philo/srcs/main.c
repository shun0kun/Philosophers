/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:19:52 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 10:17:17 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
	cfg->time_to_die = ft_atoi(argv[2]) * 1000;
	cfg->time_to_eat = ft_atoi(argv[3]) * 1000;
	cfg->time_to_sleep = ft_atoi(argv[4]) * 1000;
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
	t_shared	shared;

	if (parse_args(argc, argv, &cfg) < 0)
	{
		write(STDERR_FILENO, "Error: Invalid input\n", 21);
		return (1);
	}
	if (init_shared(&shared, &cfg) < 0)
	{
		write(STDERR_FILENO, "Error: struct s_shered init failed\n\n", 36);
		return (1);
	}
	simulation(&shared);
	destroy_shared(&shared);
	return (0);
}
