/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:35:46 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/24 17:34:17 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	eat(t_philo *args, int *ts_stop_action);
static void	go_to_sleep(t_philo *args, int *ts_stop_action);
static void	unlock_all(t_philo *args, int next_action);

/*
	The philosophers life cycle implemented as a thread routine:

	`ts_stop_action' tells us when we need to change the action we're doing.
	`next_action' tells us which is the action we need to perform next.

	while we haven't been notified that the simulation must finish,
		- check if we're busy with an action (thinking or sleeping)
		- if not, we need to change our state and do the corresponding actions

	Before we exit the function, we make sure any locked mutexes are unlocked.
*/
void	*routine(t_philo *args)
{
	int	ts_stop_action;
	int	next_action;

	ts_stop_action = args->birth;
	next_action = PH_ACTION_EAT;
	if (args->i % 2)
		usleep(10000);
	while (!get_val(args->stop))
	{
		if (ts_stop_action > ts_now())
			continue ;
		if (next_action == PH_ACTION_EAT)
			eat(args, &ts_stop_action);
		else if (next_action == PH_ACTION_SLEEP)
			go_to_sleep(args, &ts_stop_action);
		else
			log_action(PH_ACTION_THINK, args->i, args->birth, args->print_lock);
		next_action = (next_action + 1) % 3;
	}
	unlock_all(args, next_action);
	return (NULL);
}

/*
	To avoid deadlocks, even philosophers will take first the fork on
	their right and odd ones will take first the fork on their left.

	After acquiring the lock, we check wether another philosopher has died,
	because then we must not log the actions.

	update the timestamp of last meal.

	If there is only one philosopher, we do not acquire the lock, since else
	it's complicated to prevent a deadlock, and instead set time of finishing
	this action to INT_MAX.
*/
static void	eat(t_philo *args, int *ts_stop_action)
{
	if (args->params[PH_ARG_N] != 1)
	{
		pthread_mutex_lock(args->fork1);
		if (!get_val(args->stop))
			log_action(PH_ACTION_FORK, args->i, args->birth, args->print_lock);
		pthread_mutex_lock(args->fork2);
		if (!get_val(args->stop))
			log_action(PH_ACTION_FORK, args->i, args->birth, args->print_lock);
		if (!get_val(args->stop))
			log_action(PH_ACTION_EAT, args->i, args->birth, args->print_lock);
		set_val(args->last_meal, ts_now());
		*ts_stop_action = ts_now() + args->params[PH_ARG_TEAT];
	}
	else
	{
		log_action(PH_ACTION_FORK, args->i, args->birth, args->print_lock);
		*ts_stop_action = 2147483647;
	}
}

/*
	Increase the meal counter.
	Unlock both forks we used to eat.
	log the fact that we are sleeping.
	set the time of finish to now + time_to_sleep.
*/
static void	go_to_sleep(t_philo *args, int *ts_stop_action)
{
	pthread_mutex_lock(&args->meal_count->lock);
	(*args->meal_count->value)++;
	pthread_mutex_unlock(&args->meal_count->lock);
	pthread_mutex_unlock(args->fork1);
	pthread_mutex_unlock(args->fork2);
	log_action(PH_ACTION_SLEEP, args->i, args->birth, args->print_lock);
	*ts_stop_action = ts_now() + args->params[PH_ARG_TSLEEP];
}

/*
	If we were eating, unlock the forks that we got.
*/
static void	unlock_all(t_philo *args, int next_action)
{
	if (next_action == PH_ACTION_SLEEP && args->params[PH_ARG_N] != 1)
	{
		pthread_mutex_unlock(args->fork1);
		pthread_mutex_unlock(args->fork2);
	}
}
