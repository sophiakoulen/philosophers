/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/20 12:11:37 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/20 14:57:22 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	has_starved_to_death(t_philo *philo, int t_to_die);
static int	is_still_hungry(t_philo *philo, int x_eat);

/*
	Check if simulation must end.
	This is either when a philosopher has starved, or when all philosophers
	have eaten a certain amount of times.
	When we've established a philosopher is dead, we write to a shared
	variable that all philosophers must stop.
	The philosophers will regularly check that variable and die accordingly.
*/
void	watch_philos(int *params, t_philo *philos, t_protected *stop)
{
	int	continue_loop;
	int	i;

	continue_loop = 1;
	while (continue_loop)
	{
		continue_loop = 0;
		i = 0;
		while (i < params[PH_ARG_N])
		{
			if (has_starved_to_death(philos + i, params[PH_ARG_TDIE]))
			{
				continue_loop = 0;
				log_action(PH_ACTION_DIE, i, philos->ts_birth);
				break ;
			}
			if (is_still_hungry(philos + i, params[PH_ARG_XEAT]))
				continue_loop = 1;
			i++;
		}
	}
	pthread_mutex_lock(stop->lock);
	*stop->value = 1;
	pthread_mutex_unlock(stop->lock);
}

/*
	Check if a philosopher has waited for more than t_to_die milliseconds
	since the beginning of its last meal
*/
static int	has_starved_to_death(t_philo *philo, int t_to_die)
{
	int	ret;

	pthread_mutex_lock(philo->locks->last_meal + philo->i);
	ret = ts_now() - philo->ts_last_meal > t_to_die;
	pthread_mutex_unlock(philo->locks->last_meal + philo->i);
	return (ret);
}

/*
	Check if a philosopher has eaten already x_eat meals.
	If x_eat is -1, return 1.
*/
static int	is_still_hungry(t_philo *philo, int x_eat)
{
	int	ret;

	if (x_eat == -1)
		return (1);
	pthread_mutex_lock(philo->locks->meal_count + philo->i);
	ret = philo->meal_count < x_eat;
	pthread_mutex_unlock(philo->locks->meal_count + philo->i);
	return (ret);
}
