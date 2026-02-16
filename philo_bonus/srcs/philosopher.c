#include "philo_bonus.h"

void	*termination_worker(void *arg)
{
	t_termination_worker	*tw;

	tw = (t_termination_worker *)arg;
	sem_wait(tw->sem->stop);
	sem_post(tw->sem->stop);
	close_sems(tw->sem, tw->cfg->num_philos);
	free(tw->sem->meal);
	exit(0);
	return (NULL);
}

void	*monitor_starvation(void *arg)
{
	t_monitor	*monitor;

	monitor = (t_monitor *)arg;
	while (1)
	{
		sem_wait(monitor->sem->write_lock);
		sem_wait(monitor->sem->meal[monitor->id]);
		if (current_unixtime_ms() - *(monitor->last_meal_time) > monitor->cfg->time_to_die)
		{
			sem_post(monitor->sem->meal[monitor->id]);
			// sem_wait(monitor->sem->write_lock);
			printf("%lld\t%d\t%s\n", current_unixtime_ms() - monitor->start_time, monitor->id + 1, "died");
			trigger_termination(monitor->sem->stop);
			return (NULL);
		}
		sem_post(monitor->sem->meal[monitor->id]);
		sem_post(monitor->sem->write_lock);
		usleep(200); // tunable
	}
	return (NULL);
}

void	philosopher(int id, t_config *cfg, t_sem *sem, long long start_time)
{
	pthread_t	thread[2];
	t_termination_worker	tw;
	t_monitor	monitor;
	long long	last_meal_time;
	int			eat_count;
	long long	tmp;

	// init struct s_termination_worker
	tw.cfg = cfg;
	tw.sem = sem;

	// init struct s_monitor
	last_meal_time = start_time;
	monitor.id = id;
	monitor.start_time = start_time;
	monitor.last_meal_time = &last_meal_time;
	monitor.cfg = cfg;
	monitor.sem = sem;

	// create 2 threads
	pthread_create(&thread[0], NULL, termination_worker, &tw);
	pthread_detach(thread[0]);
	pthread_create(&thread[1], NULL, monitor_starvation, &monitor);
	pthread_detach(thread[1]);

	// routine
	wait_for(get_time_to_wait_in_the_beginning(id, cfg)); // tunable
	eat_count = 0;
	while (1)
	{
		sem_wait(sem->gate);
		sem_wait(sem->forks);
		sem_wait(sem->write_lock);
		printf("%lld\t%d\t%s\n", current_unixtime_ms() - start_time, id + 1, "has taken a fork");
		sem_post(sem->write_lock);
		sem_wait(sem->forks);
		sem_wait(sem->write_lock);
		printf("%lld\t%d\t%s\n", current_unixtime_ms() - start_time, id + 1, "has taken a fork");
		sem_post(sem->write_lock);


		// sem_wait(sem->meal[id]);
		// last_meal_time = current_unixtime_ms();
		// sem_post(sem->meal[id]);


		sem_wait(sem->write_lock);
		sem_wait(sem->meal[id]);
		tmp = last_meal_time;
		last_meal_time = current_unixtime_ms();
		if (last_meal_time - tmp > cfg->time_to_die)
		{
			sem_post(sem->meal[id]);
			printf("%lld\t%d\t%s\n", current_unixtime_ms() - start_time,id + 1, "died");
			trigger_termination(sem->stop);
			usleep(100000000);
		}
		printf("%lld\t%d\t%s\n", last_meal_time - start_time, id + 1, "is eating");
		sem_post(sem->meal[id]);
		sem_post(sem->write_lock);


		wait_for(cfg->time_to_eat);
		sem_post(sem->forks);
		sem_post(sem->forks);
		sem_post(sem->gate);
		eat_count++;
		if (cfg->option_enabled && eat_count >= cfg->times_to_eat)
		{
			close_sems(sem, cfg->num_philos);
			free(sem->meal);// スレッドと続けざまに実行されてダブルフリーのリスクがある。守る！けど無理じゃね。semはもう解放されてるし。どっちかだけしか実行されないようにするか、１箇所にまとめるかにする。
			exit(0);
		}
		sem_wait(sem->write_lock);
		printf("%lld\t%d\t%s\n", current_unixtime_ms() - start_time, id + 1, "is sleeping");
		sem_post(sem->write_lock);
		wait_for(cfg->time_to_sleep);
		sem_wait(sem->write_lock);
		printf("%lld\t%d\t%s\n", current_unixtime_ms() - start_time, id + 1, "is thinking");
		sem_post(sem->write_lock);
		wait_for(get_time_to_wait_on_interval(id, cfg)); // tunable
	}
	exit(0);
}
