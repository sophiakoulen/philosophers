#ifndef PHILO_H
# define PHILO_H

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <pthread.h>
#include <string.h>

/* parse errors*/

#define PH_SUCCESS 0
#define PH_NEGATIVE 1
#define PH_OVERFLOW 2

/* argument indices */

#define PH_ARG_N 0
#define PH_ARG_TDIE 1
#define PH_ARG_TEAT 2
#define PH_ARG_TSLEEP 3
#define PH_ARG_XEAT 4

/* actions */
#define PH_ACTION_FORK 0
#define PH_ACTION_EAT 1
#define PH_ACTION_SLEEP 2
#define PH_ACTION_THINK 3
#define PH_ACTION_DIE 4

struct s_philo
{
	int				i;
	int				*params;
	int				ts_birth;
	int				ts_last_meal;
	int				meal_count;
	int				*stop;
	struct s_locks	*locks;
} ;

struct s_locks
{
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_meal;
	pthread_mutex_t	*meal_count;
	pthread_mutex_t	*stop;
} ;

int		parsing(int argc, char **argv, int *params);
void	*routine(struct s_philo *args);
void	log_action(int action, int i, int ts_birth);
int		ts_now(void);
int		next_index(int i, int n);
int		first_index(int i, int n);
int		second_index(int i, int n);
void	watch_philosophers(int *params, struct s_philo *philosophers, pthread_mutex_t *stop_lock, int *stop);

#endif
