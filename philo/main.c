/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/13 16:19:52 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/16 09:52:26 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	parse_args(int argc, char **argv, t_config *cfg)
{
	if (argc != 5 && argc != 6)
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
	if (cfg->num_philos < 1 || cfg->time_to_die < 0 || cfg->time_to_eat < 0
		|| cfg->time_to_sleep < 0 || cfg->times_to_eat < 0)
		return (-1);
	return (0);
}

int	main(int argc, char **argv)
{
	t_config	cfg;
	t_shared	shared;

	if (parse_args(argc, argv, &cfg) < 0)
		return (1);
	if (init_shared(&shared, &cfg) < 0)
		return (1);
	simulation(&shared);
	destroy_shared(&shared);
	return (0);
}
