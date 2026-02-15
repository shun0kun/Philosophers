#include "philo_bonus.h"

void	*termination_worker(void *arg)
{
	t_sem	*sem;

	sem = (t_sem *)arg;
	sem_wait(sem->stop);
	sem_close(sem->forks);
	sem_close(sem->write_lock);
	sem_close(sem->stop);
	exit(0);
	return (NULL);
}

void	philosopher(int id, t_config *cfg, t_sem *sem, long long start_time)
{
	pthread_t	thread;
	int			eat_count;

	pthread_create(&thread, NULL, termination_worker, sem);
	pthread_detach(thread);
	eat_count = 0;
	while (1)
	{
		// actions
	}
	exit(0);
}

void	wait_all_children(void)
{
	while (waitpid(-1, NULL, 0) > 0)
		;
}

int	simulation(t_config *cfg, t_sem *sem)
{
	int			i;
	pid_t		pid;	
	long long	start_time;

	start_time = current_unixtime_ms();
	i = 0;
	while (i < cfg->num_philos)
	{
		pid = fork();
		if (pid == 0)
			philosopher(i, cfg, sem, start_time);
		else if (pid < 0)
		{
			broadcast_termination(sem->stop);
			wait_all_children();
			return (-1);
		}
		i++;
	}
	wait_all_children();
	return (0);
}
