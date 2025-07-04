/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 07:56:35 by tsilveir          #+#    #+#             */
/*   Updated: 2025/07/04 07:56:37 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

void	eat_philo(t_philo *philo, t_cond *cond)
{
	int	first_fork;
	int	second_fork;

	first_fork = philo->order;
	second_fork = (philo->order + 1) % cond->num_philo;
	checker_pthread_mutex_lock(&cond->arr_f_mutex[first_fork]);
	print_action(philo->order, cond, "has taken a fork");
	checker_pthread_mutex_lock(&cond->arr_f_mutex[second_fork]);
	print_action(philo->order, cond, "has taken a fork");
	checker_pthread_mutex_lock(&cond->arr_e_mutex[philo->order]);
	print_action(philo->order, cond, "is eating");
	cond->arr_time[philo->order] = get_time();
	philo->num_meals++;
	usleep(cond->time_eat * 1000);
	checker_pthread_mutex_unlock(&cond->arr_f_mutex[first_fork]);
	checker_pthread_mutex_unlock(&cond->arr_f_mutex[second_fork]);
	checker_pthread_mutex_unlock(&cond->arr_e_mutex[philo->order]);
}

void	sleep_philo(t_philo *philo, t_cond *cond)
{
	print_action(philo->order, cond, "is sleeping");
	usleep(cond->time_sleep * 1000);
}
