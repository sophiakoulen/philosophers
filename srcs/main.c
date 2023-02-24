/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:20 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/24 15:21:45 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	spawn_philos(t_philo *philos, int *params, pthread_t **threads);
static int	cleanup_mutexes(int n, struct s_locks *locks);
static int	join_and_cleanup_threads(int n, pthread_t *threads);
static int	safe_mutex_destroy(pthread_mutex_t *m);

int	main(int argc, char **argv)
{
	int				params[5];
	t_philo			*philosophers;
	pthread_t		*threads;
	struct s_locks	locks;

	if (parsing(argc, argv, params) != 0)
		return (1);
	if (init_locks(params[PH_ARG_N], &locks) != 0)
		return (2);
	if (init_philos(params, &locks, &philosophers) != 0)
		return (3);
	if (spawn_philos(philosophers, params, &threads) != 0)
		return (4);
	watch_philos(params, philosophers, &locks.stop, &locks.deadlock);
	if (join_and_cleanup_threads(params[PH_ARG_N], threads) != 0)
		return (5);
	if (cleanup_mutexes(params[PH_ARG_N], &locks) != 0)
		return (6);
	free(philosophers);
	return (0);
}

static int	spawn_philos(t_philo *philos, int *params, pthread_t **threads)
{
	int			i;

	*threads = malloc(sizeof(*threads) * params[PH_ARG_N]);
	if (!*threads)
		return (-1);
	i = 0;
	while (i < params[PH_ARG_N])
	{
		if (pthread_create(&(*threads)[i], NULL,
			(void *(*)(void *))routine, &philos[i]) != 0)
		{
			printf("Could not create thread\n");
			return (-1);
		}
		usleep(100);
		i++;
	}
	return (0);
}

static int	cleanup_mutexes(int n, struct s_locks *locks)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (safe_mutex_destroy(locks->forks + i) != 0)
			return (-1);
		if (safe_mutex_destroy(&locks->last_meal[i].lock) != 0)
			return (-1);
		free(locks->last_meal[i].value);
		if (safe_mutex_destroy(&locks->meal_count[i].lock) != 0)
			return (-1);
		free(locks->meal_count[i].value);
		if (safe_mutex_destroy(&locks->state[i].lock) != 0)
			return (-1);
		free(locks->state[i].value);
		i++;
	}
	free(locks->forks);
	free(locks->last_meal);
	free(locks->meal_count);
	safe_mutex_destroy(&locks->stop.lock);
	free(locks->stop.value);
	safe_mutex_destroy(&locks->deadlock.lock);
	free(locks->deadlock.value);
	return (0);
}

static int	join_and_cleanup_threads(int n, pthread_t *threads)
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

static int	safe_mutex_destroy(pthread_mutex_t *m)
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
