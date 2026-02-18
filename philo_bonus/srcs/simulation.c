/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   simulation.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:48 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 15:59:06 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	wait_all_children(void)
{
	while (waitpid(-1, NULL, 0) > 0)
		;
}

t_philo	get_struct_s_philo(int id, t_config *cfg, t_sem *sem,
	long long start_time)
{
	t_philo	philo;

	philo.id = id;
	philo.cfg = cfg;
	philo.sem = sem;
	philo.start_time = start_time;
	return (philo);
}

int	simulation(t_config *cfg, t_sem *sem)
{
	int			id;
	pid_t		pid;	
	long long	start_time;

	start_time = current_unixtime_us();
	id = 0;
	while (id < cfg->num_philos)
	{
		pid = fork();
		if (pid == 0)
		{
			philosopher(get_struct_s_philo(id, cfg, sem, start_time));
		}
		else if (pid < 0)
		{
			trigger_termination(sem->stop);
			wait_all_children();
			return (-1);
		}
		id++;
	}
	wait_all_children();
	return (0);
}
