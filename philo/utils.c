/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 11:49:32 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/27 10:05:38 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(int action, t_philo *args)
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
	pthread_mutex_lock(args->print_lock);
	if (!get_val(args->stop))
		printf("%dms: philo %d %s\n", ts_now() - args->birth, args->i + 1, msg);
	pthread_mutex_unlock(args->print_lock);
}

int	ts_now(void)
{
	struct timeval	t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
}

int	next_index(int i, int n)
{
	if (i == n - 1)
		return (0);
	else
		return (i + 1);
}

/*
	wrapper for getting a value from a variable protected by a mutex
*/
int	get_val(t_protected *var)
{
	int	ret;

	pthread_mutex_lock(&var->lock);
	ret = *var->value;
	pthread_mutex_unlock(&var->lock);
	return (ret);
}

/*
	wrapper for setting a variable protected by a mutex
*/
void	set_val(t_protected *var, int val)
{
	pthread_mutex_lock(&var->lock);
	*var->value = val;
	pthread_mutex_unlock(&var->lock);
}
