/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:35:46 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/06 17:35:30 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	get_ts_milli(void);
static void	log(int action, int i, int ts_birth, pthread_mutex_t *mutex);
static int	forks_available(int i, int n, int *forks);
static void	lock_forks(int i, int n, int *forks);
static void	unlock_forks(int i, int n, int *forks);
//static void	print_forks(int i, int *forks, int n);

#define PH_ACTION_FORK 0
#define PH_ACTION_EAT 1
#define PH_ACTION_SLEEP 2
#define PH_ACTION_THINK 3
#define PH_ACTION_DIE 4

void	*live(struct s_philo *args)
{
	int	i;
	int	*params;

	i = args->i;
	params = args->params;

	int	ts_birth;
	int	ts_last_meal;
	int	next_action;
	int	ts_stop_action;

	ts_birth = get_ts_milli();
	ts_last_meal = ts_birth;
	ts_stop_action = ts_birth;
	next_action = PH_ACTION_EAT;
	while (get_ts_milli() - ts_last_meal < params[PH_ARG_TDIE])
	{
		if (ts_stop_action > get_ts_milli())
			continue ;
		if (next_action == PH_ACTION_EAT)
		{
			pthread_mutex_lock(&args->mutex);

			if (!forks_available(i, params[PH_ARG_N], args->forks))
			{
				pthread_mutex_unlock(&args->mutex);
				continue ;
			}

			lock_forks(i, params[PH_ARG_N], args->forks);


			log(PH_ACTION_FORK, i, ts_birth, &args->mutex);
			log(PH_ACTION_FORK, i, ts_birth, &args->mutex);
			log(PH_ACTION_EAT, i, ts_birth, &args->mutex);
			ts_last_meal = get_ts_milli();
			next_action = PH_ACTION_SLEEP;
			ts_stop_action = get_ts_milli() + params[PH_ARG_TEAT];
		}
		else if (next_action == PH_ACTION_SLEEP)
		{
			unlock_forks(i, params[PH_ARG_N], args->forks);
			pthread_mutex_unlock(&args->mutex);

			log(PH_ACTION_SLEEP, i, ts_birth, &args->mutex);
			next_action = PH_ACTION_THINK;
			ts_stop_action = get_ts_milli() + params[PH_ARG_TSLEEP];
		}
		else
		{
			log(PH_ACTION_THINK, i, ts_birth, &args->mutex);
			next_action = PH_ACTION_EAT;
		}
	}
	log(PH_ACTION_DIE, i, ts_birth, &args->mutex);
	return (NULL);
}

static int	get_ts_milli(void)
{
	struct	timeval t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

static void	log(int action, int i, int ts_birth, pthread_mutex_t *mutex)
{
	const char	*msg;

	(void)mutex;
	msg = 0;
	if (action == PH_ACTION_FORK)
		msg = "has taken a fork";
	else if (action == PH_ACTION_EAT)
		msg = "is eating";
	else if (action == PH_ACTION_SLEEP)
		msg = "is sleeping";
	else if (action == PH_ACTION_THINK)
		msg = "is thinking";
	else if (action == PH_ACTION_DIE)
		msg = "has died";
	printf("%d %d %s\n", get_ts_milli() - ts_birth, i, msg);
}

static int	forks_available(int i, int n, int *forks)
{
	if (i != n - 1)
		return (forks[i] && forks[i + 1]);
	else
		return (forks[i] && forks[0]);
}

static void	lock_forks(int i, int n, int *forks)
{
	if (i != n - 1)
	{
		forks[i] = 0;
		forks[i + 1] = 0;
	}
	else
	{
		forks[i] = 0;
		forks[0] = 0;
	}
}

static void	unlock_forks(int i, int n, int *forks)
{
	if (i != n - 1)
	{
		forks[i] = 1;
		forks[i + 1] = 1;
	}
	else
	{
		forks[i] = 1;
		forks[0] = 1;
	}
}

/*
static void	print_forks(int i, int *forks, int n)
{
	printf("%d sees forks: ", i);
	for (int i = 0; i < n; i++)
	{
		printf("%d ", forks[i]);
	}
	printf("\n");
}
*/