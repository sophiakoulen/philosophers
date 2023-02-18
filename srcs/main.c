/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:20 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/18 15:03:39 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	has_starved_to_death(struct s_philo *philo, int t_to_die);
static int	is_still_hungry(struct s_philo *philo, int x_eat);

int	main(int argc, char **argv)
{
	int				params[5];
	int				i;
	pthread_t		*threads;
	pthread_mutex_t	*forks;
	pthread_mutex_t	*last_meal_locks;
	pthread_mutex_t	*meal_count_locks;
	struct s_philo	*philosophers;
	pthread_mutex_t	stop_lock;
	int				stop;
	int				continue_loop;

	if (parsing(argc, argv, params))
		return (1);

	printf("n: %d\n", params[PH_ARG_N]);
	printf("time_to_die: %d\n", params[PH_ARG_TDIE]);
	printf("time_to_eat: %d\n", params[PH_ARG_TEAT]);
	printf("time_to_sleep: %d\n", params[PH_ARG_TSLEEP]);
	printf("number_of_times_each_philosopher_must_eat: %d\n", params[PH_ARG_XEAT]);

	philosophers = malloc(sizeof(*philosophers) * params[PH_ARG_N]);
	forks = malloc(sizeof(*forks) * params[PH_ARG_N]);
	threads = malloc(sizeof(*threads) * params[PH_ARG_N]);
	last_meal_locks = malloc(sizeof(*last_meal_locks) * params[PH_ARG_N]);
	meal_count_locks = malloc(sizeof(*meal_count_locks) * params[PH_ARG_N]);

	if (!philosophers || !forks || !threads)
	{
		printf("Could not allocate memory\n");
		return (1);
	}

	i = 0;
	while (i < params[PH_ARG_N])
	{
		pthread_mutex_init(forks + i, NULL);
		pthread_mutex_init(last_meal_locks + i, NULL);
		pthread_mutex_init(meal_count_locks + i, NULL);
		i++;
	}

	pthread_mutex_init(&stop_lock, NULL);

	stop = 0;

	i = 0;
	while (i < params[PH_ARG_N])
	{
		philosophers[i] = (struct s_philo){i, params, forks, ts_now(), last_meal_locks + i, ts_now(), meal_count_locks + i, 0, &stop_lock, &stop};
		if (pthread_create(&threads[i], NULL, (void *(*)(void *))routine, &philosophers[i]) != 0)
		{
			printf("Could not create thread\n");
			return (1);
		}
		i++;
	}

	/*
		Check if simulation must come to an end
	*/
	continue_loop = 1;
	while (continue_loop)
	{
		continue_loop = 0;
		i = 0;
		while (i < params[PH_ARG_N])
		{
			if (has_starved_to_death(philosophers + i, params[PH_ARG_TDIE]))
			{
				log_action(PH_ACTION_DIE, i, philosophers[i].ts_birth);

				pthread_mutex_lock(&stop_lock);
				stop = 1;
				pthread_mutex_unlock(&stop_lock);
				continue_loop = 0;
				break ;
			}
			if (is_still_hungry(philosophers + i, params[PH_ARG_XEAT]))
			{
				continue_loop = 1;
			}
			i++;
		}
	}

	i = 0;
	while (i < params[PH_ARG_N])
	{
		if (pthread_join(threads[i], NULL) != 0)
		{
			printf("Could not join thread\n");
			return (1);
		}
		i++;
	}

	i = 0;
	while (i < params[PH_ARG_N])
	{
		pthread_mutex_destroy(forks + i);
		pthread_mutex_destroy(last_meal_locks + i);
		pthread_mutex_destroy(meal_count_locks + i);
		i++;
	}
	free(forks);
	free(philosophers);
	free(threads);
	return (0);
}

static int	has_starved_to_death(struct s_philo *philo, int t_to_die)
{
	int	ret;

	pthread_mutex_lock(philo->last_meal_lock);
	ret = ts_now() - philo->ts_last_meal > t_to_die;
	pthread_mutex_unlock(philo->last_meal_lock);
	return (ret);
}

static int	is_still_hungry(struct s_philo *philo, int x_eat)
{
	int	ret;

	if (x_eat == -1)
		return (1);
	pthread_mutex_lock(philo->meal_count_lock);
	ret = philo->meal_count < x_eat;
	pthread_mutex_unlock(philo->meal_count_lock);
	return (ret);
}

/*
static int	must_stop(int *stop, pthread_mutex_t *stop_lock)
{
	int	ret;

	pthread_mutex_lock(stop_lock);
	ret = *stop;
	pthread_mutex_unlock(stop_lock);
	return (ret);
}
*/