#include "philo_bonus.h"

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
			trigger_termination(sem->stop);
			wait_all_children();
			return (-1);
		}
		i++;
	}
	wait_all_children();
	return (0);
}
