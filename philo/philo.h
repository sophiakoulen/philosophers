/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 13:39:04 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/24 17:12:53 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdlib.h>
# include <stdio.h>
# include <unistd.h>
# include <sys/time.h>
# include <pthread.h>
# include <string.h>

/* parse errors*/

# define PH_SUCCESS 0
# define PH_NEGATIVE 1
# define PH_OVERFLOW 2

/* argument indices */

# define PH_ARG_N 0
# define PH_ARG_TDIE 1
# define PH_ARG_TEAT 2
# define PH_ARG_TSLEEP 3
# define PH_ARG_XEAT 4

/* actions that need to be logged */
# define PH_ACTION_EAT 0
# define PH_ACTION_SLEEP 1
# define PH_ACTION_THINK 2
# define PH_ACTION_DIE 3
# define PH_ACTION_FORK 4

/* states a philo can be in */
# define PH_STATE_ONE_FORK 42
# define PH_STATE_OTHER 0

typedef struct s_protected
{
	int				*value;
	pthread_mutex_t	lock;
}	t_protected;

typedef struct s_philo
{
	int				i;
	int				*params;
	int				birth;
	t_protected		*last_meal;
	t_protected		*meal_count;
	t_protected		*stop;
	pthread_mutex_t	*fork1;
	pthread_mutex_t	*fork2;
	pthread_mutex_t	*print_lock;
}	t_philo;

struct s_locks
{
	pthread_mutex_t	*forks;
	t_protected		*last_meal;
	t_protected		*meal_count;
	t_protected		stop;
	pthread_mutex_t	print_lock;
} ;

/* parsing.c */

int		parsing(int argc, char **argv, int *params);

/* routine.c */

void	*routine(t_philo *args);

/* utils.c */

void	log_action(int action, int i, int ts_birth, pthread_mutex_t *m);
int		ts_now(void);
int		next_index(int i, int n);
int		get_val(t_protected *var);
void	set_val(t_protected *var, int val);

/* check.c */

void	watch_philos(int *params, t_philo *philos, t_protected *stop);

/* init.c */

int		init_philos(int *params, struct s_locks *locks, t_philo **philos);
int		init_locks(int n, struct s_locks *locks);

/* destroy */

int		cleanup_mutexes(int n, struct s_locks *locks);
int		join_and_cleanup_threads(int n, pthread_t *threads);
int		safe_mutex_destroy(pthread_mutex_t *m);

#endif
