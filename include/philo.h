#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>
#define	MAX_PHILOSOPHERS

typedef struct s_conditions
{
	int	num_philo;
	int	time_die;
	int	time_eat;
	int	time_sleep;
	int	must_eat;
	int	end_flag;
	pthread_mutex_t *arr_mutex;
	pthread_mutex_t print;
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
size_t	get_time(void);
