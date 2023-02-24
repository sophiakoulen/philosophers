/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 14:50:17 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/24 13:42:13 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	init_protected(t_protected *var, int value);
static int	safe_mutex_init(pthread_mutex_t *m);

int	init_philos(int *params, struct s_locks *locks, t_philo **philos)
{
	int				i;

	*philos = malloc(sizeof(**philos) * params[PH_ARG_N]);
	if (!*philos)
		return (-1);
	i = 0;
	while (i < params[PH_ARG_N])
	{
		(*philos)[i].i = i;
		(*philos)[i].params = params;
		(*philos)[i].birth = ts_now();
		(*philos)[i].last_meal = &locks->last_meal[i];
		*(*philos)[i].last_meal->value = ts_now();
		(*philos)[i].meal_count = &locks->meal_count[i];
		*(*philos)[i].meal_count->value = 0;
		(*philos)[i].state = &locks->state[i];
		(*philos)[i].stop = &locks->stop;
		(*philos)[i].deadlock = &locks->deadlock;
		(*philos)[i].fork1 = &locks->forks[first_index(i, params[PH_ARG_N])];
		(*philos)[i].fork2 = &locks->forks[second_index(i, params[PH_ARG_N])];
		i++;
	}
	return (0);
}

int	init_locks(int n, struct s_locks *locks)
{
	int				i;

	locks->forks = malloc(sizeof(*locks->forks) * n);
	locks->last_meal = malloc(sizeof(*locks->last_meal) * n);
	locks->meal_count = malloc(sizeof(*locks->meal_count) * n);
	locks->state = malloc(sizeof(*locks->state) * n);
	if (!locks->meal_count || !locks->last_meal || !locks->meal_count || !locks->state)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (safe_mutex_init(locks->forks + i) != 0)
			return (-1);
		if (init_protected(&locks->last_meal[i], -1) != 0)
			return (-1);
		if (init_protected(&locks->meal_count[i], -1) != 0)
			return (-1);
		if (init_protected(&locks->state[i], -1) != 0)
			return (-1);
		i++;
	}
	if (init_protected(&locks->stop, 0) != 0)
		return (-1);
	if (init_protected(&locks->deadlock, 0) != 0)
		return (-1);
	return (0);
}

static int	safe_mutex_init(pthread_mutex_t *m)
{
	int	ret;

	ret = pthread_mutex_init(m, NULL);
	if (ret != 0)
	{
		printf("failed to initialize mutex, error code: %d", ret);
		return (-1);
	}
	return (0);
}

static int	init_protected(t_protected *var, int value)
{
	if (safe_mutex_init(&var->lock))
		return (-1);
	var->value = malloc(sizeof(*var->value));
	if (!var->value)
		return (-1);
	*var->value = value;
	return (0);
}
