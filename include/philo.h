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
	pthread_mutex_t *arr_mutex;
	pthread_mutex_t print;
	struct timeval *arr_time;
	struct timeval str_time;
}	t_conditions;

typedef struct s_philo
{
	pthread_t	TID;
	int			order;
	int			num_meals;
}	t_philo;
