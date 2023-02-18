/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:35:46 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/18 14:57:42 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(struct s_philo *args, int *ts_stop_action);
static void	go_to_sleep(struct s_philo *args, int *ts_stop_action);
static int	is_alive(struct s_philo *args);
//static void	die(struct s_philo *args, int ts_birth, int next_action);

/*
	The philosophers life cycle implemented as a thread routine:

	while we haven't been notified of another's philosophers death,
	- check if we are dead
	- update some internal state and lock / unlock mutexes if needed
*/
void	*routine(struct s_philo *args)
{
	int	ts_stop_action; //timestamp we will be finished doing our current action
	int	next_action; // next action we need to do

	ts_stop_action = args->ts_birth;
	next_action = PH_ACTION_EAT; //first thing we need to do is eat
	while (is_alive(args))
	{
		if (ts_stop_action > ts_now()) //if we're already doing an action
			continue ;
		if (next_action == PH_ACTION_EAT)
		{
			eat(args, &ts_stop_action);
			//this blocks the execution => problem: what if we die here??
			next_action = PH_ACTION_SLEEP;
		}
		else if (next_action == PH_ACTION_SLEEP)
		{
			go_to_sleep(args, &ts_stop_action);
			next_action = PH_ACTION_THINK;
		}
		else
		{
			log_action(PH_ACTION_THINK, args->i, args->ts_birth);
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

/*
	To avoid deadlocks, even philosophers will take first the fork on
	their right and odd ones will take first the fork on their left.

	After acquiring the lock, we check wether another philosopher has died,
	because then we must not log the action.

	update the timestamp of last meal.
*/
static void	eat(struct s_philo *args, int *ts_stop_action)
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
	log_action(PH_ACTION_FORK, args->i, args->ts_birth);
	pthread_mutex_lock(args->forks + index2);
	log_action(PH_ACTION_FORK, args->i, args->ts_birth);
	log_action(PH_ACTION_EAT, args->i, args->ts_birth);
	pthread_mutex_lock(args->last_meal_lock);
	args->ts_last_meal = ts_now();
	pthread_mutex_unlock(args->last_meal_lock);
	*ts_stop_action = ts_now() + args->params[PH_ARG_TEAT];
}

/*
	Unlock both forks we used to eat.
*/
static void	go_to_sleep(struct s_philo *args, int *ts_stop_action)
{
	pthread_mutex_lock(args->meal_count_lock);
	args->meal_count++;
	pthread_mutex_unlock(args->meal_count_lock);
	pthread_mutex_unlock(args->forks + args->i);
	pthread_mutex_unlock(args->forks + next_index(args->i, args->params[PH_ARG_N]));
	log_action(PH_ACTION_SLEEP, args->i, args->ts_birth);
	*ts_stop_action = ts_now() + args->params[PH_ARG_TSLEEP];
}

static int	is_alive(struct s_philo *args)
{
	int	ret;

	pthread_mutex_lock(args->stop_lock);
	ret = !*args->stop;
	pthread_mutex_unlock(args->stop_lock);
	return (ret);
}

/*
	If we were eating, unlock the forks that we got.
	Tell the other philosophers this philosopher is dead.
*/
/*
static void	die(struct s_philo *args, int next_action)
{
	log(PH_ACTION_DIE, args->i, ts_birth);
	if (next_action == PH_ACTION_SLEEP)
	{
		pthread_mutex_unlock(args->forks + args->i);
		pthread_mutex_unlock(args->forks + next_index(args->i, args->params[PH_ARG_N]));
	}
	*args->death = 1;
}
*/
