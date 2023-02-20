/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 14:50:17 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/20 14:57:41 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static struct s_locks	*init_locks(int n);

struct s_philo	*init_philos(int *params, t_protected *stop)
{
	t_philo			*philos;
	struct s_locks	*locks;
	int				i;

	locks = init_locks(params[PH_ARG_N]);
	if (!locks)
		return (NULL);
	philos = malloc(sizeof(*philos) * params[PH_ARG_N]);
	if (!philos)
		return (NULL);
	i = 0;
	while (i < params[PH_ARG_N])
	{
		philos[i].i = i;
		philos[i].params = params;
		philos[i].ts_birth = ts_now();
		philos[i].ts_last_meal = ts_now();
		philos[i].meal_count = 0;
		philos[i].stop = stop;
		philos[i].locks = locks;
		i++;
	}
	return (philos);
}

int	init_stop_lock(t_protected *stop)
{
	stop->lock = malloc(sizeof(*stop->lock));
	if (!stop->lock)
		return (-1);
	pthread_mutex_init(stop->lock, NULL);
	stop->value = malloc(sizeof(*stop->value));
	*stop->value = 0;
	return (0);
}

static struct s_locks	*init_locks(int n)
{
	struct s_locks	*locks;
	int				i;

	locks = malloc(sizeof(*locks));
	if (!locks)
		return (NULL);
	locks->forks = malloc(sizeof(*locks->forks) * n);
	locks->last_meal = malloc(sizeof(*locks->last_meal) * n);
	locks->meal_count = malloc(sizeof(*locks->meal_count) * n);
	if (!locks->meal_count || !locks->last_meal || !locks->meal_count)
		return (NULL);
	i = 0;
	while (i < n)
	{
		pthread_mutex_init(locks->forks + i, NULL);
		pthread_mutex_init(locks->last_meal + i, NULL);
		pthread_mutex_init(locks->meal_count + i, NULL);
		i++;
	}
	return (locks);
}
