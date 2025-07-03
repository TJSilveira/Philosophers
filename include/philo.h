#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

typedef struct s_conditions
{
	int	num_philo;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	must_eat;
	int	end_flag;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t *arr_f_mutex;
	pthread_mutex_t *arr_e_mutex;
	pthread_mutex_t print_mutex;
	size_t *arr_time;
	size_t str_time;
}	t_conditions;

typedef struct s_philo
{
	pthread_t	TID;
	int			order;
	int			num_meals;
}	t_philo;

/* Functions */
int	ft_atoi_simple(const char *nptr);
size_t	get_time(void);
