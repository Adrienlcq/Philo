/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:10:10 by adlecler          #+#    #+#             */
/*   Updated: 2022/09/22 15:07:29 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	init_philo(t_info *info)
{
	int	i;

	i = -1;
	while (++i < info->nb_philo)
	{
		if (pthread_mutex_init(&info->fork[i], NULL) != 0) //creer un mutex fourchette pour chaque philo
		{
			
			printf("Error\nMMutex init failed\n");
			return (0);
		}
		info->philo[i].id = i; //id du philo
		info->philo[i].fork_l = i;
		info->philo[i].fork_r = (i + 1) % info->nb_philo; //pour le dernier philo, la fourchette droite est la premiere fourchette
		info->philo[i].info = info; //copie de la structure info dans la structure philo
	}
	return (1);
}

int	init_mutex(t_info *info)
{
	int	i;

	i = -1;
	if (pthread_mutex_init(&info->print, NULL) != 0) //creer un mutex pour l'affichage
	{
		printf("Error\nMutex init failed\n");
		return (0);
	}
	if (pthread_mutex_init(&info->dead, NULL) != 0) //creer un mutex pour la mort
	{
		printf("Error\nMutex init failed\n");
		return (0);
	}
	if (pthread_mutex_init(&info->eat, NULL) != 0) //creer un mutex pour le nombre de fois qu'un philo a mange
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

int	recover_info(char **av, t_info *info)
{
	if (!ft_check_overflow(av))
		return (0);
	info->nb_philo = ft_atol(av[1]);
	info->time_to_die = ft_atol(av[2]);
	info->time_to_eat = ft_atol(av[3]);
	info->time_to_sleep = ft_atol(av[4]);
	if (!check_nb_philo(info->nb_philo))
		return (0);
	if (av[5])
		info->nb_must_eat = ft_atol(av[5]);
	else
		info->nb_must_eat = -1;
	info->is_dead = 0;
	return (1);
}

int	parse_arg(char **av, t_info *info)
{
	int i;

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

int main(int ac, char **av)
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
