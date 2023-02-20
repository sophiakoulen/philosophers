/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:20 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/20 14:59:14 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static pthread_t		*spawn_philos(t_philo *philos, int *params);
static void				cleanup_mutexes(int n, struct s_locks *locks);
static void				join_and_cleanup_threads(int n, pthread_t *threads);
static void				destroy_philos(int n, t_philo *philosophers);

int	main(int argc, char **argv)
{
	int				params[5];
	t_philo			*philosophers;
	pthread_t		*threads;
	t_protected		stop;

	if (parsing(argc, argv, params))
		return (1);
	if (init_stop_lock(&stop) != 0)
		return (2);
	philosophers = init_philos(params, &stop);
	if (!philosophers)
		return (2);
	threads = spawn_philos(philosophers, params);
	if (!threads)
		return (2);
	watch_philos(params, philosophers, &stop);
	join_and_cleanup_threads(params[PH_ARG_N], threads);
	destroy_philos(params[PH_ARG_N], philosophers);
	return (0);
}

static pthread_t	*spawn_philos(t_philo *philos, int *params)
{
	pthread_t	*threads;
	int			i;

	threads = malloc(sizeof(*threads) * params[PH_ARG_N]);
	if (!threads)
		return (NULL);
	i = 0;
	while (i < params[PH_ARG_N])
	{
		if (pthread_create(&threads[i], NULL,
				(void *(*)(void *))routine, &philos[i]) != 0)
		{
			printf("Could not create thread\n");
		}
		i++;
	}
	return (threads);
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
	free(locks->forks);
	free(locks->last_meal);
	free(locks->meal_count);
}

static void	destroy_philos(int n, t_philo *philosophers)
{
	struct s_locks	*locks;

	if (n == 0)
		return ;
	locks = philosophers[0].locks;
	cleanup_mutexes(n, locks);
	free(philosophers);
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
