/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 13:39:04 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/21 13:53:28 by skoulen          ###   ########.fr       */
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

/* actions */
# define PH_ACTION_EAT 0
# define PH_ACTION_SLEEP 1
# define PH_ACTION_THINK 2
# define PH_ACTION_DIE 3
# define PH_ACTION_FORK 4

typedef struct s_protected
{
	int				*value;
	pthread_mutex_t	*lock;
}	t_protected;

typedef struct s_philo
{
	int				i;
	int				*params;
	int				ts_birth;
	int				ts_last_meal;
	int				meal_count;
	t_protected		*stop;
	struct s_locks	*locks;
}	t_philo;

struct s_locks
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_meal;
	pthread_mutex_t	*meal_count;
	pthread_mutex_t	*stop;
} ;

/* parsing.c */

int				parsing(int argc, char **argv, int *params);

/* routine.c */

void			*routine(t_philo *args);

/* utils.c */

void			log_action(int action, int i, int ts_birth);
int				ts_now(void);
int				next_index(int i, int n);
int				first_index(int i, int n);
int				second_index(int i, int n);

/* check.c */

void			watch_philos(int *params, t_philo *philos, t_protected *stop);

/* init.c */

struct s_philo	*init_philos(int *params, t_protected *stop);
int				init_stop_lock(t_protected *stop);

#endif
