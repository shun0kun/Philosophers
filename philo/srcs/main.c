#include "philo.h"

void	*philosopher(void *arg)
{
	t_philo_state	state;
	int				eat_count;

	
	return (NULL);
}

int	generate_philosophers(t_philo_ctx *ctx)
{
	int	i;

	i = 0;
	while (i < ctx->number_of_philosophers)
	{
		pthread_create(&ctx->threads[i], NULL, philosopher, ctx);
		i++;
	}
	return (0);
}

int	kill_philosophers(t_philo_ctx *ctx)
{
	int	i;

	if (!ctx->threads)
		return (-1);
	i = 0;
	while (i < ctx->number_of_philosophers)
	{
		pthread_join(ctx->threads[i], NULL);
		i++;
	}
	return (0);
}

int	philo_ctx_init(t_philo_ctx *ctx, char **argv, int option_flag)
{
	ctx->number_of_philosophers = ft_atoi(argv[1]);
	ctx->time_to_die = ft_atoi(argv[2]);
	ctx->time_to_eat = ft_atoi(argv[3]);
	ctx->time_to_sleep = ft_atoi(argv[4]);
	if (option_flag)
		ctx->number_of_times_each_philosopher_must_eat = ft_atoi(argv[5]);	
	ctx->threads = malloc(sizeof(pthread_t) * ctx->number_of_philosophers);
	if (!ctx->threads)
		return (-1);
	ctx->fork_state = malloc(sizeof(t_fork_state) * ctx->number_of_philosophers);
	if (!ctx->fork_state)
		return (-1);
}

void	philo_ctx_destroy(t_philo_ctx *ctx)
{
	int	i;

	if (!ctx)
		return ;
	if (ctx->threads)
	{
		i = 0;
		while (i < ctx->number_of_philosophers)
		{
			free(ctx->threads[i]);
			i++;
		}
	}
	if (ctx->fork_state)
	{
		i = 0;
		while (i < ctx->number_of_philosophers)
		{
			free(ctx->fork_state[i]);
			i++;
		}
	}
}

int	main(int argc, char **argv)
{
	t_philo_ctx	ctx;
	int			option_flag;

	if (argc < 5 || argv > 6)
		return (1);
	option_flag = 0;
	if (argc == 6)
		option_flag = 1;
	if (philo_ctx_init(&ctx, argv, option_flag) < 0)
		return (1);
	pthread_mutex_init(&ctx.mtx, NULL);
	genarate_philosophers(&ctx);
	kill_philosophers(&ctx);
	pthread_mutex_destroy(&ctx.mtx);
	philo_ctx_destroy(&ctx);
	return (0);
}
