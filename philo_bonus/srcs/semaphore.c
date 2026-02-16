#include "philo_bonus.h"

void	fill_id_to_meal_sem_name(char *dest, int id)
{
	int	dig;

	dig = get_num_len(id);
	dest[dig] = '\0';
	while (dig)
	{
		dest[dig - 1] = id % 10 + '0';
		id = id / 10;
		dig--;
	}
}

bool	sem_open_all_success(t_sem *sem, int num_philos)
{
	int	id;

	if (!sem->forks || sem->forks == SEM_FAILED)
		return (false);
	if (!sem->gate || sem->gate == SEM_FAILED)
		return (false);
	if (!sem->write_lock || sem->write_lock == SEM_FAILED)
		return (false);
	if (!sem->stop || sem->stop == SEM_FAILED)
		return (false);
	id = 0;
	while (id < num_philos)
	{
		if (!sem->meal[id] || sem->meal[id] == SEM_FAILED)
			return (false);
		id++;
	}
	return (true);
}

int	init_sems(t_sem *sem, int num_philos)
{
	static char	meal_sem_name[] = "/mealXXXXXXXXXX";
	int			id;

	sem->forks = NULL;
	sem->gate = NULL;
	sem->stop = NULL;
	sem->write_lock = NULL;
	sem->meal = malloc(sizeof(sem_t *) * (num_philos + 1));
	if (!sem->meal)
		return (-1);
	id = 0;
	while (id <= num_philos)
		sem->meal[id++] = NULL;
	unlink_sems(num_philos);
	sem->forks = sem_open("/forks", O_CREAT | O_EXCL, 0644, num_philos);
	if (num_philos == 1)
		sem->gate = sem_open("/gate", O_CREAT | O_EXCL, 0644, 1);
	else
		sem->gate = sem_open("/gate", O_CREAT | O_EXCL, 0644, num_philos / 2);
	sem->write_lock = sem_open("/write_lock", O_CREAT | O_EXCL, 0644, 1);
	sem->stop = sem_open("/stop", O_CREAT | O_EXCL, 0644, 0);
	id = 0;
	while (id < num_philos)
	{
		fill_id_to_meal_sem_name(&meal_sem_name[5], id);
		sem->meal[id] = sem_open(meal_sem_name, O_CREAT | O_EXCL, 0644, 1);
		id++;
	}
	if (sem_open_all_success(sem, num_philos) == false)
	{
		close_sems(sem, num_philos);
		free(sem->meal);
		unlink_sems(num_philos);
		return (-1);
	}
	return (0);
}

void	close_sems(t_sem *sem, int num_philos)
{
	int	id;

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
	if (sem->meal)
	{
		id = 0;
		while (id < num_philos)
		{
			if (sem->meal[id] && sem->meal[id] != SEM_FAILED)
				sem_close(sem->meal[id]);
			id++;
		}
	}
}

void	unlink_sems(int num_philos)
{
	static char	meal_sem_name[] = "/mealXXXXXXXXXX";
	int			id;

	sem_unlink("/forks");
	sem_unlink("/gate");
	sem_unlink("/write_lock");
	sem_unlink("/stop");
	id = 0;
	while (id < num_philos)
	{
		fill_id_to_meal_sem_name(&meal_sem_name[5], id);
		sem_unlink(meal_sem_name);
		id++;
	}
}
