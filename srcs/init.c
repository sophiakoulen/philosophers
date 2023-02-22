/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 14:50:17 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/22 14:15:23 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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
		(*philos)[i].stop = &locks->stop;
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
	if (!locks->meal_count || !locks->last_meal || !locks->meal_count)
		return (-1);
	i = 0;
	while (i < n)
	{
		if (pthread_mutex_init(locks->forks + i, NULL))
			return (-1);
		if (pthread_mutex_init(&locks->last_meal[i].lock, NULL))
			return (-1);
		locks->last_meal[i].value = malloc(sizeof(int));
		if (!locks->last_meal[i].value)
			return (-1);
		if (pthread_mutex_init(&locks->meal_count[i].lock, NULL))
			return (-1);
		locks->meal_count[i].value = malloc(sizeof(int));
		if (!locks->meal_count[i].value)
			return (-1);
		i++;
	}
	if (pthread_mutex_init(&locks->stop.lock, NULL))
		return (-1);
	locks->stop.value = malloc(sizeof(int));
	if (!locks->stop.value)
		return (-1);
	*locks->stop.value = 0;
	return (0);
}
