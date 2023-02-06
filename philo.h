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

struct s_philo
{
	int				i;
	int				*params;
	int				*forks;
	pthread_mutex_t	mutex;
} ;

int		parsing(int argc, char **argv, int *params);
void	*live(struct s_philo *args);

#endif
