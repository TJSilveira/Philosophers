#ifndef PHILO_H
# define PHILO_H
# include <stdio.h>
# include <string.h>
# include <unistd.h>
# include <stdlib.h>
# include <sys/time.h>
# include <pthread.h>

typedef struct s_conditions
{
	int				num_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				must_eat;
	int				end_flag;
	pthread_mutex_t	end_mutex;
	pthread_mutex_t	*arr_f_mutex;
	pthread_mutex_t	*arr_e_mutex;
	pthread_mutex_t	print_mutex;
	size_t			*arr_time;
	size_t			str_time;
}	t_conditions;

typedef struct s_philo
{
	pthread_t	tid;
	int			order;
	int			num_meals;
}	t_philo;

/* philo.c */
void	*engine(void *arg);
int	reached_eat_goal(t_conditions *cond, t_philo **philo);
int	checker_philo_status(t_conditions *cond, int i);
void	*observer_func(void *arg);

/* init_functions.c */
void	init_conditions_aux(t_conditions *c, int argc, char *argv[]);
void	init_conditions(t_conditions *c, int argc, char *argv[]);
t_conditions	*cond_func(void);
void	spawn_philosophers(t_conditions *cond, t_philo **philo);
void	engine_1philo(t_conditions *cond);


/* utils_1.c */
int			ft_isdigit(int c);
void		check_argv(int argc, char *argv[]);
void		check_num_philo(t_conditions *c, char *argv[]);
int			ft_atoi_simple(const char *nptr);
void		error_handler(char *err_msg);

/* utils_2.c */
void	checker_pthread_mutex_lock(pthread_mutex_t *mutex);
void	checker_pthread_mutex_unlock(pthread_mutex_t *mutex);
void	print_action(int order, t_conditions *cond, char *msg);
size_t	get_time(void);
void	despawn_philosophers(t_conditions *cond, t_philo **philo, pthread_t *observer);

/* actions.c */
void	eat_philo(t_philo *philo, t_conditions *cond);
void	sleep_philo(t_philo *philo, t_conditions *cond);

#endif