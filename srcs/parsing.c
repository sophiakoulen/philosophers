/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: skoulen <skoulen@student.42lausanne.ch>    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/02/06 12:18:16 by skoulen           #+#    #+#             */
/*   Updated: 2023/02/06 12:34:05 by skoulen          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

static int	ft_atoi(char *str, int *res);
static int	check_argc(int argc);

int	parsing(int argc, char **argv, int *params)
{
	int	i;
	int	err;

	if (check_argc(argc) != 0)
		return (1);
	err = 0;
	i = 0;
	while (i < 5)
	{
		if (i == 4 && argc == 5)
		{
			params[i] = -1;
			break ;
		}
		err = ft_atoi(argv[i + 1], &params[i]);
		if (err)
		{
			printf("%s: error\n", argv[i + 1]);
			return (1);
		}
		i++;
	}
	return (0);
}

static int	check_argc(int argc)
{
	if (argc != 5 && argc != 6)
	{
		printf("usage: ./philo number_of_philosophers time_to_die time_to_eat\
			time_to_sleep [number_of_times_each_philosopher_must_eat");
		return (1);
	}
	return (0);
}

static int	ft_atoi(char *str, int *res)
{
	int	tmp;

	if (*str == '-')
		return (PH_NEGATIVE);
	if (*str == '+')
		str++;
	*res = 0;
	while (*str >= '0' && *str <= '9')
	{
		tmp = *res * 10;
		if (tmp / 10 != *res)
			return (PH_OVERFLOW);
		*res = tmp;
		*res += *str - '0';
		if (*res < 0)
			return (PH_OVERFLOW);
		str++;
	}
	return (0);
}
