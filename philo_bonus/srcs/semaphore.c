/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   semaphore.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sshimots <sshimots@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/02/18 09:24:44 by sshimots          #+#    #+#             */
/*   Updated: 2026/02/18 09:53:00 by sshimots         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

bool	sem_open_all_success(t_sem *sem)
{
	if (!sem->forks || sem->forks == SEM_FAILED)
		return (false);
	if (!sem->gate || sem->gate == SEM_FAILED)
		return (false);
	if (!sem->write_lock || sem->write_lock == SEM_FAILED)
		return (false);
	if (!sem->stop || sem->stop == SEM_FAILED)
		return (false);
	return (true);
}

int	init_sems(t_sem *sem, int num_philos)
{
	unlink_sems();
	sem->forks = NULL;
	sem->gate = NULL;
	sem->stop = NULL;
	sem->write_lock = NULL;
	sem->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, num_philos);
	if (num_philos == 1)
		sem->gate = sem_open("/gate", O_CREAT | O_EXCL, 0644, 1);
	else
		sem->gate = sem_open("/gate", O_CREAT | O_EXCL, 0644, num_philos / 2);
	sem->write_lock = sem_open("/write_lock", O_CREAT | O_EXCL, 0644, 1);
	sem->stop = sem_open("/stop", O_CREAT | O_EXCL, 0644, 0);
	if (sem_open_all_success(sem) == false)
	{
		close_sems(sem);
		unlink_sems();
		return (-1);
	}
	return (0);
}

void	close_sems(t_sem *sem)
{
	if (!sem)
		return ;
	if (sem->forks && sem->forks != SEM_FAILED)
	{
		sem_close(sem->forks);
		sem->forks = NULL;
	}
	if (sem->gate && sem->gate != SEM_FAILED)
	{
		sem_close(sem->gate);
		sem->gate = NULL;
	}
	if (sem->write_lock && sem->write_lock != SEM_FAILED)
	{
		sem_close(sem->write_lock);
		sem->write_lock = NULL;
	}
	if (sem->stop && sem->stop != SEM_FAILED)
	{
		sem_close(sem->stop);
		sem->stop = NULL;
	}
}

void	unlink_sems(void)
{
	sem_unlink("/forks");
	sem_unlink("/gate");
	sem_unlink("/write_lock");
	sem_unlink("/stop");
}
