#include "philo_bonus.h"

void	*termination_worker(void *arg)
{
	t_sem	*sem;

	sem = (t_sem *)arg;
	sem_wait(sem->stop);
	sem_post(sem->stop);
	sem_close(sem->forks); // if (sem->forks) {sem_close(sem->forks); sem_forks = NULL}
	sem_close(sem->write_lock);
	sem_close(sem->stop);
	// destroy meal sems
	// free heap. do not double free
	exit(0);
	return (NULL);
}

void	*monitor_starvation(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	while (1)
	{
		sem_wait(monitor->sem->meal);
		if (current_unixtime_ms() - *(monitor->last_meal_time) > monitor->cfg->time_to_die)
		{
			sem_post(monitor->sem->meal);
			sem_wait(monitor->sem->write_lock);
			printf("%d\t%lld\t%s\n", monitor->id, current_unixtime_ms() - monitor->start_time, "died");
			trigger_termination(monitor->sem->stop);
			return (NULL);
		}
		sem_post(monitor->sem->meal);
		usleep(500); // tunable
	}
	return (NULL);
}

void	philosopher(int id, t_config *cfg, t_sem *sem, long long start_time)
{
	pthread_t	thread[2];
	t_monitor	monitor;
	long long	last_meal_time;
	int			eat_count;

	// init struct s_monitor
	last_meal_time = start_time;
	monitor.id = id;
	monitor.start_time = start_time;
	monitor.last_meal_time = &last_meal_time;
	monitor.cfg = cfg;
	monitor.sem = sem;

	// create 2 threads
	pthread_create(&thread[0], NULL, termination_worker, sem);
	pthread_detach(&thread[0]);
	pthread_create(&thread[1], NULL, monitor_starvation, &monitor);
	pthread_detach(&thread[1]);

	// routine
	wait_for(); // tunable
	eat_count = 0;
	while (1)
	{
		sem_wait(sem->gate);
		sem_wait(sem->forks);
		sem_wait(sem->write_lock);
		printf("%d\t%lld\t%s\n", id, current_unixtime_ms() - start_time, "has taken a fork");
		sem_post(sem->write_lock);
		sem_wait(sem->forks);
		sem_wait(sem->write_lock);
		printf("%d\t%lld\t%s\n", id, current_unixtime_ms() - start_time, "has taken a fork");
		sem_post(sem->write_lock);
		sem_wait(&sem->meal[id]);
		last_meal_time = current_unixtime_ms();
		sem_post(&sem->meal[id]);
		sem_wait(sem->write_lock);
		printf("%d\t%lld\t%s\n", id, current_unixtime_ms() - start_time, "is eating");
		sem_post(sem->write_lock);
		wait_for();
		sem_post(sem->forks);
		sem_post(sem->forks);
		sem_post(sem->gate);
		eat_count++;
		if (eat_count >= cfg->times_to_eat)
		{
			// close, free, etc. do not double free.
			exit(0);
		}
		sem_wait(sem->write_lock);
		printf("%d\t%lld\t%s\n", id, current_unixtime_ms() - start_time, "is sleeping");
		sem_post(sem->write_lock);
		wait_for();
		sem_wait(sem->write_lock);
		printf("%d\t%lld\t%s\n", id, current_unixtime_ms() - start_time, "is thinking");
		sem_post(sem->write_lock);
		wait_for();
	}
	exit(0);
}
