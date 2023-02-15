/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:35:46 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/15 13:41:45 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ts_now(void);
static int	next_index(int i, int n);
static void	log(int action, int i, int ts_birth);
static void	eat(struct s_philo *args, int ts_birth, int *ts_last_meal, int *ts_stop_action);
static void	go_to_sleep(struct s_philo *args, int ts_birth, int *ts_stop_action);
static void	die(struct s_philo *args, int ts_birth, int next_action);

#define PH_ACTION_FORK 0
#define PH_ACTION_EAT 1
#define PH_ACTION_SLEEP 2
#define PH_ACTION_THINK 3
#define PH_ACTION_DIE 4

/*
	The philosophers life cycle implemented as a thread routine:

	while we haven't been notified of another's philosophers death,
	- check if we are dead
	- update some internal state and lock / unlock mutexes if needed
*/
void	*routine(struct s_philo *args)
{
	int	ts_birth; // timestamp of birth
	int	ts_last_meal; // timestamp of last meal
	int	ts_stop_action; //timestamp we will be finished doing our current action
	int	next_action; // next action we need to do

	ts_birth = ts_now();
	ts_last_meal = ts_birth;
	ts_stop_action = ts_birth;
	next_action = PH_ACTION_EAT; //first thing we need to do is eat
	while (!*args->death)
	{
		if (ts_now() - ts_last_meal >= args->params[PH_ARG_TDIE])
		{
			die(args, ts_birth, next_action);
			return (NULL);
		}
		if (ts_stop_action > ts_now()) //if we're already doing an action
			continue ;
		if (next_action == PH_ACTION_EAT)
		{
			eat(args, ts_birth, &ts_last_meal, &ts_stop_action);
			next_action = PH_ACTION_SLEEP;
		}
		else if (next_action == PH_ACTION_SLEEP)
		{
			go_to_sleep(args, ts_birth, &ts_stop_action);
			next_action = PH_ACTION_THINK;
		}
		else
		{
			log(PH_ACTION_THINK, args->i, ts_birth);
			next_action = PH_ACTION_EAT;
		}
	}
	if (next_action == PH_ACTION_SLEEP)
	{
		pthread_mutex_unlock(args->forks + args->i);
		pthread_mutex_unlock(args->forks + next_index(args->i, args->params[PH_ARG_N]));
	}
	return (NULL);
}

static int	ts_now(void)
{
	struct	timeval t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

static int	next_index(int i, int n)
{
	if (i == n - 1)
		return (0);
	else
		return (i + 1);
}

static void	log(int action, int i, int ts_birth)
{
	const char	*msg;

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
	printf("%d %d %s\n", ts_now() - ts_birth, i, msg);
}

/*
	To avoid deadlocks, even philosophers will take first the fork on
	their right and odd ones will take first the fork on their left.

	After acquiring the lock, we check wether another philosopher has died,
	because then we must not log the action.

	update the timestamp of last meal.
*/
static void	eat(struct s_philo *args, int ts_birth, int *ts_last_meal, int *ts_stop_action)
{
	int	index1;
	int	index2;

	if (args->i % 2)
	{
		index1 = args->i;
		index2 = next_index(args->i, args->params[PH_ARG_N]);

	}
	else
	{
		index2 = args->i;
		index1 = next_index(args->i, args->params[PH_ARG_N]);
	}
	pthread_mutex_lock(args->forks + index1);
	if (!*args->death)
		log(PH_ACTION_FORK, args->i, ts_birth);
	pthread_mutex_lock(args->forks + index2);
	if (!*args->death)
	{
		log(PH_ACTION_FORK, args->i, ts_birth);
		log(PH_ACTION_EAT, args->i, ts_birth);
	}
	*ts_last_meal = ts_now();
	*ts_stop_action = ts_now() + args->params[PH_ARG_TEAT];
}

/*
	Unlock both forks we used to eat.
*/
static void	go_to_sleep(struct s_philo *args, int ts_birth, int *ts_stop_action)
{
	pthread_mutex_unlock(args->forks + args->i);
	pthread_mutex_unlock(args->forks + next_index(args->i, args->params[PH_ARG_N]));
	log(PH_ACTION_SLEEP, args->i, ts_birth);
	*ts_stop_action = ts_now() + args->params[PH_ARG_TSLEEP];
}

/*
	If we were eating, unlock the forks that we got.
	Tell the other philosophers this philosopher is dead.
*/
static void	die(struct s_philo *args, int ts_birth, int next_action)
{
	log(PH_ACTION_DIE, args->i, ts_birth);
	if (next_action == PH_ACTION_SLEEP)
	{
		pthread_mutex_unlock(args->forks + args->i);
		pthread_mutex_unlock(args->forks + next_index(args->i, args->params[PH_ARG_N]));
	}
	*args->death = 1;
}
