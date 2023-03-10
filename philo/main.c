/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:20 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/24 17:13:21 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	spawn_philos(t_philo *philos, int *params, pthread_t **threads);

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
	watch_philos(params, philosophers, &locks.stop);
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
		i++;
	}
	return (0);
}
