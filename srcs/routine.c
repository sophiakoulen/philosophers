/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:35:46 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/20 12:48:15 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// index of the philosopher
#define I args->i

// number of philosophers
#define N args->params[0]

static void	eat(struct s_philo *args, int *ts_stop_action);
static void	go_to_sleep(struct s_philo *args, int *ts_stop_action);
static int	is_alive(struct s_philo *args);
static void	die(struct s_philo *args, int next_action);

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
	die(args, next_action);
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
	log_action(PH_ACTION_FORK, I, args->ts_birth);
	if (args->params[PH_ARG_N] != 1)
	{
		pthread_mutex_lock(args->locks->forks + first_index(I, N));
		pthread_mutex_lock(args->locks->forks + second_index(I, N));
		log_action(PH_ACTION_FORK, I, args->ts_birth);
		log_action(PH_ACTION_EAT, I, args->ts_birth);
		pthread_mutex_lock(args->locks->last_meal + I);
		args->ts_last_meal = ts_now();
		pthread_mutex_unlock(args->locks->last_meal + I);
		*ts_stop_action = ts_now() + args->params[PH_ARG_TEAT];
	}
	else
	{
		*ts_stop_action = 2147483647;
	}
}

/*
	Unlock both forks we used to eat.
*/
static void	go_to_sleep(struct s_philo *args, int *ts_stop_action)
{
	pthread_mutex_lock(args->locks->meal_count + I);
	args->meal_count++;
	pthread_mutex_unlock(args->locks->meal_count + I);
	pthread_mutex_unlock(args->locks->forks + I);
	pthread_mutex_unlock(args->locks->forks + next_index(I, N));
	log_action(PH_ACTION_SLEEP, args->i, args->ts_birth);
	*ts_stop_action = ts_now() + args->params[PH_ARG_TSLEEP];
}

static int	is_alive(struct s_philo *args)
{
	int	ret;

	pthread_mutex_lock(args->locks->stop);
	ret = !*args->stop;
	pthread_mutex_unlock(args->locks->stop);
	return (ret);
}

/*
	If we were eating, unlock the forks that we got.
*/
static void	die(struct s_philo *args, int next_action)
{
	if (next_action == PH_ACTION_SLEEP && N != 0)
	{
		pthread_mutex_unlock(args->locks->forks + I);
		pthread_mutex_unlock(args->locks->forks + next_index(I, N));
	}
}
