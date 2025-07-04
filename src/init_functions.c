/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_functions.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 07:56:52 by tsilveir          #+#    #+#             */
/*   Updated: 2025/07/04 07:56:53 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	init_conditions_aux(t_cond *c, int argc, char *argv[])
{
	int	i;

	if (argc == 6)
		c->must_eat = ft_atoi_simple(argv[5]);
	else
		c->must_eat = -1;
	c->arr_f_mutex = malloc(sizeof(pthread_mutex_t) * c->num_philo);
	c->arr_e_mutex = malloc(sizeof(pthread_mutex_t) * c->num_philo);
	c->arr_time = malloc(sizeof(size_t) * c->num_philo);
	if (!c->arr_f_mutex || !c->arr_e_mutex || !c->arr_time)
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

void	init_conditions(t_cond *c, int argc, char *argv[])
{
	if (argc == 5 || argc == 6)
	{
		check_argv(argc, argv);
		check_input(&c->num_philo, argv, 1, "Philosphers not valid");
		check_input(&c->time_die, argv, 2, "Time to die not valid");
		check_input(&c->time_eat, argv, 3, "Time to eat not valid");
		check_input(&c->time_sleep, argv, 4, "Time to sleep not valid");
		c->end_flag = 0;
		init_conditions_aux(c, argc, argv);
	}
	else
	{
		printf("Number of arguments is not valid\n");
		exit(EXIT_FAILURE);
	}
}

t_cond	*cond_func(void)
{
	static t_cond	cond;

	return (&cond);
}

void	spawn_philosophers(t_cond *cond, t_philo **philo)
{
	int	i;

	i = 0;
	while (i < cond->num_philo)
	{
		philo[i] = malloc(sizeof(t_philo));
		if (!philo[i])
			exit(EXIT_FAILURE);
		philo[i]->order = i;
		philo[i]->num_meals = 0;
		if (pthread_create(&philo[i]->tid, NULL, &engine, philo[i]) != 0)
			exit(EXIT_FAILURE);
		usleep(100);
		i++;
	}
}

void	engine_1philo(t_cond *cond)
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
