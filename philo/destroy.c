/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/24 17:10:49 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/24 17:19:10 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	protected_destroy(t_protected *var);

int	cleanup_mutexes(int n, struct s_locks *locks)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (safe_mutex_destroy(locks->forks + i) != 0)
			return (-1);
		if (protected_destroy(&locks->last_meal[i]) != 0)
			return (-1);
		if (protected_destroy(&locks->meal_count[i]) != 0)
			return (-1);
		i++;
	}
	free(locks->forks);
	free(locks->last_meal);
	free(locks->meal_count);
	if (protected_destroy(&locks->stop) != 0)
		return (-1);
	if (safe_mutex_destroy(&locks->print_lock) != 0)
		return (-1);
	return (0);
}

int	join_and_cleanup_threads(int n, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			printf("Could not join thread\n");
			return (-1);
		}
		i++;
	}
	free(threads);
	return (0);
}

int	safe_mutex_destroy(pthread_mutex_t *m)
{
	int	ret;

	ret = pthread_mutex_destroy(m);
	if (ret != 0)
	{
		printf("failed to destroy mutex: error code: %d", ret);
		return (-1);
	}
	return (0);
}

static int	protected_destroy(t_protected *var)
{
	if (safe_mutex_destroy(&var->lock) != 0)
		return (-1);
	free(var->value);
	return (0);
}
