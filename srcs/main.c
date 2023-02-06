/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:20 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/06 17:11:42 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int argc, char **argv)
{
	int				params[5];
	int				i;
	int				*forks;
	pthread_t		*threads;
	pthread_mutex_t	mutex;
	struct s_philo	*philosophers;

	if (parsing(argc, argv, params))
		return (1);

	printf("n: %d\n", params[PH_ARG_N]);
	printf("time_to_die: %d\n", params[PH_ARG_TDIE]);
	printf("time_to_eat: %d\n", params[PH_ARG_TEAT]);
	printf("time_to_sleep: %d\n", params[PH_ARG_TSLEEP]);
	printf("number_of_times_each_philosopher_must_eat: %d\n", params[PH_ARG_XEAT]);

	philosophers = malloc(sizeof(*philosophers) * params[PH_ARG_N]);

	forks = malloc(sizeof(*forks) * params[PH_ARG_N]);
	memset(forks, 1, sizeof(*forks) * params[PH_ARG_N]);

	threads = malloc(sizeof(*threads) * params[PH_ARG_N]);

	if (!threads)
	{
		printf("Could not allocate memory\n");
		return (1);
	}

	pthread_mutex_init(&mutex, NULL);

	i = 0;
	while (i < params[PH_ARG_N])
	{
		philosophers[i].i = i;
		philosophers[i].params = params;
		philosophers[i].forks = forks;
		philosophers[i].mutex = mutex;

		if (pthread_create(&threads[i], NULL, (void *(*)(void *))live, &philosophers[i]) != 0)
		{
			printf("Could not create thread\n");
			//free threads??
			return (1);
		}
		i++;
	}
	i = 0;
	while (i < params[PH_ARG_N])
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			printf("Could not join thread\n");
			//free threads??
			return (1);
		}
		i++;
	}

	pthread_mutex_destroy(&mutex);

	return (0);
}
