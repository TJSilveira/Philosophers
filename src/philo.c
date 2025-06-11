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
				exit(1);
			}
			j++;
		}
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
			exit(1);
		}
		i++;
	}
	return ((int)total);
}

void	init_conditions(t_conditions *c, int argc, char *argv[])
{
	int i;

	if (argc == 5 || argc == 6)
	{
		check_argv(argc, argv);
		c->num_philo = ft_atoi_simple(argv[1]);
		c->time_die = ft_atoi_simple(argv[2]);
		c->time_eat = ft_atoi_simple(argv[3]);
		c->time_sleep = ft_atoi_simple(argv[4]);
		if (argc == 6)
			c->must_eat = ft_atoi_simple(argv[5]);
		else
			c->must_eat = -1;
		i = -1;
		c->arr_mutex = malloc(sizeof(pthread_mutex_t) * c->num_philo);
		if (c->arr_mutex == NULL)
			exit(1);
		while (++i < c->num_philo + 1)
			pthread_mutex_init(&c->arr_mutex[i], NULL);
		c->arr_time = malloc(sizeof(struct timeval) * c->num_philo);
		if (c->arr_time == NULL)
			exit(1);
	}
	else
	{
		printf("Number of arguments is not valid\n");
		exit(1);
	}
}

t_conditions	*cond_func()
{
	static t_conditions cond;
	return (&cond);
}

/*Fork 0 is the fork to the left of the philosopher 0*/
void	eat_philo(t_philo *philo, t_conditions *cond)
{
	int fork0;
	int fork1;

	fork0 = philo->order;
	fork1 = (philo->order + 1) % cond->num_philo;
	// cond->arr_mutex[order];
}

void	*engine(void *arg)
{
	t_philo 		*philo;
	t_conditions	*cond;
	
	philo = (t_philo*)arg;
	cond = cond_func();

	while (1)
	{
		eat_philo(philo, cond);
		// think_philo;
		// sleep_philo;
	}
	
	printf("Order: %i; Thread ID:%li; Number of times to eat: %i\n", philo->order, philo->TID, cond->must_eat);
	return (NULL);
}

int	main(int argc,char *argv[])
{
	t_philo			**philo;
	t_conditions	*cond;

	cond = cond_func();
	init_conditions(cond, argc, argv);

	// Make malloc array to have the address/TID of all the philosopher	
	philo = malloc(sizeof(t_philo*) * cond->num_philo);

	int	i;

	i = 0;
	while (i < cond->num_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		philo[i]->order = i;
		pthread_create(&philo[i]->TID, NULL, &engine, philo[i]);
		printf("%d\n", i);
		i++;
	}
	i = 0;
	while (i < cond->num_philo)
	{
		pthread_join(philo[i]->TID, NULL);
		printf("%d\n", i);
		i++;
	}
	// Loop to create all the threads. Put the TID of each thread in the malloc array
}