/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/02 18:48:56 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/02 18:51:53 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	recover_info(char **av, t_info *info)
{
	if (!ft_check_overflow(av))
		return (0);
	info->nb_philo = ft_atol(av[1]);
	info->time_to_die = ft_atol(av[2]);
	info->time_to_eat = ft_atol(av[3]);
	info->time_to_sleep = ft_atol(av[4]);
	info->is_dead = 0;
	info->is_full = 0;
	info->nb_thread_detached = 0;
	if (!check_nb_philo(info->nb_philo))
		return (0);
	if (av[5])
		info->nb_must_eat = ft_atol(av[5]);
	else
		info->nb_must_eat = -1;
	info->is_dead = 0;
	return (1);
}

int	philo_dead(int *check, int i, t_info *info)
{
	*check = 1;
	pthread_mutex_unlock(&info->last_meal);
	print_status(info, i, "died", 1);
	pthread_mutex_lock(&info->dead);
	info->is_dead = 1;
	pthread_mutex_unlock(&info->dead);
	return (0);
}

void	unlock_forks(t_info *info, t_philo *philo, int nb)
{
	if (nb == 1)
		pthread_mutex_unlock(&info->fork[philo->fork_l]);
	else if (nb == 2)
	{
		pthread_mutex_unlock(&info->fork[philo->fork_l]);
		pthread_mutex_unlock(&info->fork[philo->fork_r]);
	}
}
