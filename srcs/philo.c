/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:10:10 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/03 10:23:37 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_philo(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->nb_philo)
	{
		if (pthread_mutex_init(&info->fork[i], NULL) != 0) // initialisation de quel mutex ?
		{
			printf("Error\nMutex init failed\n");
			return (0);
		}
		info->philo[i].id = i;
		info->philo[i].fork_l = i;
		if (i == info->nb_philo - 1)
			info->philo[i].fork_r = 0;
		else
			info->philo[i].fork_r = i + 1 % info->nb_philo;
		info->philo[i].info = info;
		info->philo[i].last_meal = 0;
		info->philo[i].nb_meals = 0;
	}
	return (1);
}

int	init_mutex(t_info *info)
{
	int	i; // pas besoin ???

	i = -1;
	if (pthread_mutex_init(&info->print, NULL) != 0)
	{
		printf("Error\nMutex init failed\n");
		return (0);
	}
	if (pthread_mutex_init(&info->dead, NULL) != 0)
	{
		printf("Error\nMutex init failed\n");
		return (0);
	}
	if (pthread_mutex_init(&info->last_meal, NULL) != 0)
	{
		printf("Error\nMutex init failed\n");
		return (0);
	}
	if (pthread_mutex_init(&info->eat, NULL) != 0)
	{
		printf("Error\nMutex init failed\n");
		return (0);
	}
	return (1);
}

int	check_nb_philo(int nb_philo)
{
	if (nb_philo > 200)
	{
		printf("Error\n200 philosophers max\n");
		return (0);
	}
	if (nb_philo == 0)
	{
		printf("Error\nNeed one or more philosophers\n");
		return (0);
	}
	return (1);
}

int	parse_arg(char **av, t_info *info)
{
	int	i;

	i = 0;
	while (av[++i])
	{
		if (!ft_isdigit(av[i]))
		{
			printf("Error\nInvalid number (numeric character only)\n");
			return (0);
		}
	}
	if (!recover_info(av, info))
		return (0);
	return (1);
}

int	main(int ac, char **av)
{
	t_info	info;

	if (ac != 5 && ac != 6)
	{
		printf("Error\nNeed 5 or 6 arguments\n");
		return (1);
	}
	if (!parse_arg(av, &info))
		return (1);
	if (!(init_philo(&info)) || !(init_mutex(&info)))
		return (1);
	if (!(start_philo(&info)))
		return (1);
	return (0);
}
