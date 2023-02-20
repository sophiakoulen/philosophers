/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/18 11:49:32 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/20 13:53:15 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	log_action(int action, int i, int ts_birth)
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
	printf("%dms:\tphilo nr. %d %s\n", ts_now() - ts_birth, i + 1, msg);
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

int	first_index(int i, int n)
{
	if (i % 2)
	{
		return (i);
	}
	else
	{
		return (next_index(i, n));
	}
}

int	second_index(int i, int n)
{
	if (i % 2)
	{
		return (next_index(i, n));
	}
	else
	{
		return (i);
	}
}
