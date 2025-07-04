/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 07:56:43 by tsilveir          #+#    #+#             */
/*   Updated: 2025/07/04 07:56:44 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	*engine(void *arg)
{
	t_philo	*philo;
	t_cond	*cond;
	int		i;

	philo = (t_philo *)arg;
	cond = cond_func();
	cond->arr_time[philo->order] = get_time();
	i = 0;
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

int	reached_eat_goal(t_cond *cond, t_philo **philo)
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

int	checker_philo_status(t_cond *cond, int i)
{
	size_t			curr_time;

	curr_time = get_time();
	if ((cond->arr_time[i] + cond->time_die) < curr_time)
	{
		if (pthread_mutex_trylock(&cond->arr_e_mutex[i]) == 0)
		{
			checker_pthread_mutex_lock(&cond->end_mutex);
			cond->end_flag = 1;
			print_action(i, cond, "died");
			checker_pthread_mutex_unlock(&cond->end_mutex);
			checker_pthread_mutex_unlock(&cond->arr_e_mutex[i]);
			return (1);
		}
	}
	return (0);
}

void	*observer_func(void *arg)
{
	t_cond	*cond;
	t_philo	**philo;
	int		i;

	cond = cond_func();
	philo = (t_philo **)arg;
	usleep(100 * cond->num_philo);
	while (1)
	{
		i = 0;
		while (i < cond->num_philo)
		{
			if (checker_philo_status(cond, i))
				return (NULL);
			i++;
			if (cond->must_eat != -1)
			{
				if (reached_eat_goal(cond, philo))
					return (NULL);
			}
		}
		usleep(5);
	}
	return (NULL);
}

int	main(int argc, char *argv[])
{
	t_philo		**philo;
	t_cond		*cond;
	pthread_t	observer;

	cond = cond_func();
	init_conditions(cond, argc, argv);
	engine_1philo(cond);
	philo = malloc(sizeof(t_philo *) * cond->num_philo);
	if (!philo)
		exit(EXIT_FAILURE);
	spawn_philosophers(cond, philo);
	if (pthread_create(&observer, NULL, &observer_func, philo) != 0)
		exit(EXIT_FAILURE);
	despawn_philosophers(cond, philo, &observer);
	return (0);
}
