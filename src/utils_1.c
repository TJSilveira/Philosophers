/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_1.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tsilveir <tsilveir@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/07/04 07:57:02 by tsilveir          #+#    #+#             */
/*   Updated: 2025/07/04 07:57:03 by tsilveir         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philo.h"

int	ft_isdigit(int c)
{
	if (c >= '0' && c <= '9')
		return (1);
	else
		return (0);
}

void	check_argv(int argc, char *argv[])
{
	int	i;
	int	j;

	i = 0;
	while (++i < argc)
	{
		j = 0;
		while (argv[i][j])
		{
			if (ft_isdigit(argv[i][j]) == 0)
			{
				printf("The argument '%s' is not valid\n", argv[i]);
				exit(EXIT_FAILURE);
			}
			j++;
		}
	}
}

void	check_num_philo(t_cond *c, char *argv[])
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
	int			i;

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
