/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:20 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/20 12:50:16 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static void	init_locks(int n, struct s_locks *locks);
static void	cleanup_mutexes(int n, struct s_locks *locks);
static void	join_and_cleanup_threads(int n, pthread_t *threads);

int	main(int argc, char **argv)
{
	int				params[5];
	int				i;
	struct s_philo	*philosophers;
	pthread_t		*threads;
	struct s_locks	locks;
	int				stop;

	if (parsing(argc, argv, params))
		return (1);
	philosophers = malloc(sizeof(*philosophers) * params[PH_ARG_N]);
	threads = malloc(sizeof(*threads) * params[PH_ARG_N]);

	init_locks(params[PH_ARG_N], &locks);

	stop = 0;

	i = 0;
	int birth = ts_now();
	while (i < params[PH_ARG_N])
	{
		philosophers[i] = (struct s_philo){i,
			params,
			birth, //birth
			birth, //last_meal
			0, //meal_count
			&stop, //must we stop?
			&locks}; // all the locks we need
		if (pthread_create(&threads[i], NULL, (void *(*)(void *))routine, &philosophers[i]) != 0)
		{
			printf("Could not create thread\n");
			return (1);
		}
		i++;
	}
	watch_philosophers(params, philosophers, locks.stop, &stop);
	join_and_cleanup_threads(params[PH_ARG_N], threads);
	cleanup_mutexes(params[PH_ARG_N], &locks);
	free(philosophers);
	return (0);
}

static void	init_locks(int n, struct s_locks *locks)
{
	int	i;

	locks->forks = malloc(sizeof(*locks->forks) * n);
	locks->last_meal = malloc(sizeof(*locks->last_meal) * n);
	locks->meal_count = malloc(sizeof(*locks->meal_count) * n);
	locks->stop = malloc(sizeof(*locks->stop) * 1);
	i = 0;
	while (i < n)
	{
		pthread_mutex_init(locks->forks + i, NULL);
		pthread_mutex_init(locks->last_meal + i, NULL);
		pthread_mutex_init(locks->meal_count + i, NULL);
		i++;
	}
	pthread_mutex_init(locks->stop, NULL);
}

static void	cleanup_mutexes(int n, struct s_locks *locks)
{
	int	i;
	int	ret;

	i = 0;
	while (i < n)
	{
		ret = pthread_mutex_destroy(locks->forks + i);
		if (ret != 0)
			printf("could not destroy mutex, error: %d\n", ret);
		ret = pthread_mutex_destroy(locks->last_meal + i);
		if (ret != 0)
			printf("could not destroy mutex, error: %d\n", ret);
		ret = pthread_mutex_destroy(locks->meal_count + i);
		if (ret != 0)
			printf("could not destroy mutex, error: %d\n", ret);
		i++;
	}
	pthread_mutex_destroy(locks->stop);
	free(locks->forks);
	free(locks->last_meal);
	free(locks->meal_count);
	free(locks->stop);
}

static void	join_and_cleanup_threads(int n, pthread_t *threads)
{
	int	i;

	i = 0;
	while (i < n)
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			printf("Could not join thread\n");
		}
		i++;
	}
	free(threads);
}
