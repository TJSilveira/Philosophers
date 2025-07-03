#include "../include/philo.h"

int ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

void	check_argv(int argc, char *argv[])
{
	int i;
	int	j;

	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if(ft_isdigit(argv[i][j]) == 0)
			{
				printf("The argument '%s' is not valid\n", argv[i]);
				exit(EXIT_FAILURE);
			}
			j++;
		}
	}
}

void	check_num_philo(t_conditions *c, char *argv[])
{
	c->num_philo = ft_atoi_simple(argv[1]);
	if (c->num_philo < 1)
	{
		printf("Invalid number of philosophers '%s'\n", argv[1]);
		exit(EXIT_FAILURE);
	}
}

int	ft_atoi_simple(const char *nptr)
{
	long int	total;
	int				i;

	total = 0;
	i = 0;
	if (!(nptr[i] >= '0' && nptr[i] <= '9'))
		return (-1);
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		total = total * 10 + nptr[i] - '0';
		if (total > __INT_MAX__)
		{
			printf("%s exceeds INT_MAX\n", nptr);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	return ((int)total);
}

void	error_handler(char *err_msg)
{
	printf("Error: %s\n", err_msg);
	exit (EXIT_FAILURE);
}

void	init_conditions(t_conditions *c, int argc, char *argv[])
{
	int i;

	if (argc == 5 || argc == 6)
	{
		check_argv(argc, argv);
		check_num_philo(c, argv);
		c->time_die = ft_atoi_simple(argv[2]);
		c->time_eat = ft_atoi_simple(argv[3]);
		c->time_sleep = ft_atoi_simple(argv[4]);
		c->end_flag = 0;
		if (argc == 6)
			c->must_eat = ft_atoi_simple(argv[5]);
		else
			c->must_eat = -1;
		c->arr_f_mutex = malloc(sizeof(pthread_mutex_t) * c->num_philo);
		c->arr_e_mutex = malloc(sizeof(pthread_mutex_t) * c->num_philo);
		c->arr_time = malloc(sizeof(size_t) * c->num_philo);
		if(!c->arr_f_mutex || !c->arr_e_mutex || !c->arr_time)
			exit(EXIT_FAILURE);
		i = -1;
		while (++i < c->num_philo)
		{
			pthread_mutex_init(&c->arr_f_mutex[i], NULL);
			pthread_mutex_init(&c->arr_e_mutex[i], NULL);
		}
		memset(c->arr_time, 0, sizeof(size_t) * c->num_philo);
		pthread_mutex_init(&c->print_mutex, NULL);
		pthread_mutex_init(&c->end_mutex, NULL);
		c->str_time = get_time();
	}
	else
	{
		printf("Number of arguments is not valid\n");
		exit(EXIT_FAILURE);
	}
}

t_conditions	*cond_func()
{
	static t_conditions cond;
	return (&cond);
}

void	print_action(int order, t_conditions *cond, char *msg)
{
	size_t	curr_time;

	if(pthread_mutex_lock(&cond->print_mutex) != 0)
		error_handler("Error locking the standard output\n");
	if (!(msg[0] != 'd' && cond->end_flag == 1))
	{
		curr_time = get_time();
		printf("%ld %i %s\n", curr_time - cond->str_time, order + 1, msg);
	}
	if(pthread_mutex_unlock(&cond->print_mutex) != 0)
		error_handler("Error unlocking the standard output\n");
}

size_t	get_time(void)
{
	struct timeval tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	eat_philo(t_philo *philo, t_conditions *cond)
{
	int first_fork;
	int second_fork;

	// if (philo->order % 2 == 0)
	// {
	// 	first_fork = philo->order;
	// 	second_fork = (philo->order + 1) % cond->num_philo;
	// }
	// else
	// {
	// 	first_fork= (philo->order + 1) % cond->num_philo;
	// 	second_fork = philo->order;
	// }
	first_fork = philo->order;
	second_fork = (philo->order + 1) % cond->num_philo;
	pthread_mutex_lock(&cond->arr_f_mutex[first_fork]);
	print_action(philo->order, cond, "has taken a fork");
	pthread_mutex_lock(&cond->arr_f_mutex[second_fork]);
	print_action(philo->order, cond, "has taken a fork");
	pthread_mutex_lock(&cond->arr_e_mutex[philo->order]);
	print_action(philo->order, cond, "is eating");
	cond->arr_time[philo->order] = get_time();
	philo->num_meals++;
	usleep(cond->time_eat * 1000);
	if(pthread_mutex_unlock(&cond->arr_f_mutex[first_fork]) != 0)
		error_handler("Error unlocking the left fork\n");
	if(pthread_mutex_unlock(&cond->arr_f_mutex[second_fork]) != 0)
		error_handler("Error unlocking the right fork\n");
	if(pthread_mutex_unlock(&cond->arr_e_mutex[philo->order]) != 0)
		error_handler("Error unlocking the right fork\n");
}

void	sleep_philo(t_philo *philo, t_conditions *cond)
{
	print_action(philo->order, cond, "is sleeping");
	usleep(cond->time_sleep * 1000);
}

void	*engine(void *arg)
{
	t_philo 		*philo;
	t_conditions	*cond;
	
	philo = (t_philo*)arg;
	cond = cond_func();
	cond->arr_time[philo->order] = get_time();
	int i = 0;
	while (1)
	{
		eat_philo(philo, cond);
		sleep_philo(philo, cond);
		print_action(philo->order, cond, "is thinking");
		if (cond->end_flag == 1)
			return (NULL);
		i++;
	}	
	return (NULL);
}

int	reached_eat_goal(t_conditions *cond, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < cond->num_philo)
	{
		if (philo[i]->num_meals < cond->must_eat)
			return (0);
		i++;
	}
	cond->end_flag = 1;
	return (1);
}

void	*observer_func(void* arg)
{
	t_conditions	*cond;
	t_philo			**philo;
	size_t			curr_time;
	int				i;

	cond = cond_func();
	philo = (t_philo**)arg;
	usleep(100 * cond->num_philo);
	while (1)
	{
		i = 0;
		while (i < cond->num_philo)
		{
			curr_time = get_time();
			if ((cond->arr_time[i] + cond->time_die) < curr_time)
			{
				if(pthread_mutex_trylock(&cond->arr_e_mutex[i]) == 0)
				{
					pthread_mutex_lock(&cond->end_mutex);
					cond->end_flag = 1;
					print_action(i, cond, "died");
					if(pthread_mutex_unlock(&cond->end_mutex) != 0)
						error_handler("Error unlocking the right fork\n");
					if(pthread_mutex_unlock(&cond->arr_e_mutex[i]) != 0)
						error_handler("Error unlocking the right fork\n");
					return (NULL);
				}
			}
			i++;
			if (cond->must_eat != -1)
			{
				if (reached_eat_goal(cond, philo))
				{
					return (NULL);
				}
			}
		}
		usleep(5);
	}
	return (NULL);
}

void	engine_1philo(t_conditions* cond)
{
	if (cond->num_philo == 1)
	{
		print_action(0, cond, "has taken a fork");
		usleep(1000 * cond->time_die);
		print_action(0, cond, "died");
		free(cond->arr_f_mutex);
		free(cond->arr_time);
		exit(0);
	}
}

int	main(int argc,char *argv[])
{
	t_philo			**philo;
	t_conditions	*cond;
	pthread_t		observer;
	int	i;

	cond = cond_func();
	init_conditions(cond, argc, argv);
	engine_1philo(cond);
	philo = malloc(sizeof(t_philo*) * cond->num_philo);
	if(!philo)
		exit(EXIT_FAILURE);
	i = 0;
	while (i < cond->num_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			exit(EXIT_FAILURE);
		philo[i]->order = i;
		philo[i]->num_meals = 0;
		if(pthread_create(&philo[i]->TID, NULL, &engine, philo[i]) != 0)
			exit(EXIT_FAILURE);
		usleep(100);
		i++;
	}
	if(pthread_create(&observer, NULL, &observer_func, philo) != 0)
		exit(EXIT_FAILURE);
	i = 0;
	pthread_join(observer, NULL);
	while (i < cond->num_philo)
	{
		pthread_join(philo[i]->TID, NULL);
		free(philo[i]);
		pthread_mutex_destroy(&cond->arr_f_mutex[i]);
		pthread_mutex_destroy(&cond->arr_e_mutex[i]);
		i++;
	}
	pthread_mutex_destroy(&cond->print_mutex);
	free(philo);
	free(cond->arr_f_mutex);
	free(cond->arr_e_mutex);
	free(cond->arr_time);
}