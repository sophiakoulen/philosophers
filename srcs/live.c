/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   live.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:35:46 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/06 15:46:17 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	get_ts_milli(void);
static void	log(int action, int i, int ts_birth);

#define PH_ACTION_FORK 0
#define PH_ACTION_EAT 1
#define PH_ACTION_SLEEP 2
#define PH_ACTION_THINK 3
#define PH_ACTION_DIE 4

void	*live(struct s_args *args)
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
			//if both not both forks available
				//continue;

			//lock 2 forks

			log(PH_ACTION_FORK, i, ts_birth);
			log(PH_ACTION_FORK, i, ts_birth);
			log(PH_ACTION_EAT, i, ts_birth);
			ts_last_meal = get_ts_milli();
			next_action = PH_ACTION_SLEEP;
			ts_stop_action = get_ts_milli() + params[PH_ARG_TEAT];
		}
		else if (next_action == PH_ACTION_SLEEP)
		{
			log(PH_ACTION_SLEEP, i, ts_birth);
			next_action = PH_ACTION_THINK;
			ts_stop_action = get_ts_milli() + params[PH_ARG_TSLEEP];
		}
		else
		{
			log(PH_ACTION_THINK, i, ts_birth);
			next_action = PH_ACTION_EAT;
		}
	}
	log(PH_ACTION_DIE, i, ts_birth);
	return (NULL);
}

static int	get_ts_milli(void)
{
	struct	timeval t;

	gettimeofday(&t, NULL);
	return (t.tv_sec * 1000 + t.tv_usec / 1000);
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
	printf("%d %d %s\n", get_ts_milli() - ts_birth, i, msg);
}
