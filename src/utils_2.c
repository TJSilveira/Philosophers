#include "../include/philo.h"

void	checker_pthread_mutex_lock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_lock(mutex) != 0)
		error_handler("Error locking mutex\n");
}

void	checker_pthread_mutex_unlock(pthread_mutex_t *mutex)
{
	if (pthread_mutex_unlock(mutex) != 0)
		error_handler("Error unlocking mutex\n");
}

void	print_action(int order, t_cond *cond, char *msg)
{
	size_t	curr_time;

	checker_pthread_mutex_lock(&cond->print_mutex);
	if (!(msg[0] != 'd' && cond->end_flag == 1))
	{
		curr_time = get_time();
		printf("%ld %i %s\n", curr_time - cond->str_time, order + 1, msg);
	}
	checker_pthread_mutex_unlock(&cond->print_mutex);
}

size_t	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (tv.tv_sec * 1000 + tv.tv_usec / 1000);
}

void	despawn_philosophers(t_cond *cond, t_philo **philo, pthread_t *observer)
{
	int	i;

	i = 0;
	pthread_join(*observer, NULL);
	while (i < cond->num_philo)
	{
		pthread_join(philo[i]->tid, NULL);
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
