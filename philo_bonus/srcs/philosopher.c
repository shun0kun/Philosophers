/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:38 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/20 11:42:03 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	trigger_termination(sem_t *stop)
{
	sem_post(stop);
}

void	*termination_worker(void *arg)
{
	t_termination_worker	*tw;

	tw = (t_termination_worker *)arg;
	sem_wait(tw->sem->stop);
	sem_post(tw->sem->stop);
	close_sems(tw->sem);
	exit(0);
	return (NULL);
}

void	*monitor_starvation(void *arg)
{
	t_monitor	*monitor;
	long long	now;

	monitor = (t_monitor *)arg;
	while (1)
	{
		sem_wait(monitor->sem->write_lock);
		now = current_unixtime_us();
		if (now - *(monitor->last_eat_time)
			> monitor->cfg->time_to_die)
		{
			printf("%lld\t%d\t%s\n",
				utom(now - monitor->start_time), monitor->id + 1, "died");
			trigger_termination(monitor->sem->stop);
			return (NULL);
		}
		sem_post(monitor->sem->write_lock);
		usleep(300);
	}
	return (NULL);
}

void	create_and_detatch_threads(t_philo *philo,
	t_termination_worker *tw, t_monitor *monitor)
{
	pthread_t				thread[2];

	tw->sem = philo->sem;
	monitor->id = philo->id;
	monitor->start_time = philo->start_time;
	monitor->last_eat_time = &philo->last_eat_time;
	monitor->cfg = philo->cfg;
	monitor->sem = philo->sem;
	pthread_create(&thread[0], NULL, termination_worker, tw);
	pthread_detach(thread[0]);
	pthread_create(&thread[1], NULL, monitor_starvation, monitor);
	pthread_detach(thread[1]);
}

void	philosopher(t_philo philo)
{
	t_termination_worker	tw;
	t_monitor				monitor;

	if (philo.cfg->option_enabled && philo.cfg->times_to_eat == 0)
	{
		close_sems(philo.sem);
		exit(0);
	}
	philo.last_eat_time = philo.start_time;
	create_and_detatch_threads(&philo, &tw, &monitor);
	wait_for(time_to_wait_first(&philo));
	philo.eat_count = 0;
	while (1)
	{
		philo_eat(&philo);
		philo_sleep(&philo);
		philo_think(&philo);
		wait_for(time_to_wait_on_interval(&philo));
	}
	exit(0);
}
