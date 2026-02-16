#include <stdio.h>

int	get_num_len(int nb)
{
	int	count;

	if (nb == 0)
		return (1);
	count = 0;
	while (nb > 0)
	{
		nb = nb / 10;
		count++;
	}
	return (count);
}

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

void	unlink_sems(int num_philos)
{
	static char	meal_sem_name[] = "/mealXXXXXXXXXX";
	int			id;
	int			dig;

	id = 0;
	while (id < num_philos)
	{
		fill_id_to_meal_sem_name(&meal_sem_name[5], id);
		printf("%s\n", meal_sem_name);
		id++;
	}
}


int	main(void)
{
	unlink_sems(1000);

	return (0);
}
