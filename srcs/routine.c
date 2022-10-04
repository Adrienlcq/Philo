/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:02:50 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/03 20:37:46 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_death(t_info *info)
{
	int		i;
	t_philo	*philo;
	int		check;

	philo = info->philo;
	check = 0;
	while (1)
	{
		i = -1;
		while (++i < info->nb_philo && info->is_dead == 0)
		{
			if (info->nb_must_eat >= 0)
				if (check_meals(info, philo) == 1)
					return (1);
			pthread_mutex_lock(&info->last_meal);
			if (ft_get_time() - philo[i].last_meal > info->time_to_die)
				return (philo_dead(&check, i, info));
			if (check == 0)
				pthread_mutex_unlock(&info->last_meal);
			usleep(200);
		}
	}
	return (1);
}

void	eat(t_philo *philo, t_info *info)
{
	pthread_mutex_lock(&(info->fork[philo->fork_l]));
	if (print_status(info, philo->id, "has taken a fork", 0) == -1)
		return (unlock_forks(info, philo, 1));
	if (info->nb_philo == 1)
	{
		ft_usleep(info->time_to_die + 5, info);
		return (unlock_forks(info, philo, 1));
	}
	pthread_mutex_lock(&(info->fork[philo->fork_r]));
	if (print_status(info, philo->id, "has taken a fork", 0) == -1)
		return (unlock_forks(info, philo, 2));
	if (print_status(info, philo->id, "is eating", 0) == -1)
		return (unlock_forks(info, philo, 2));
	pthread_mutex_lock(&info->eat);
	philo->nb_meals++;
	pthread_mutex_unlock(&info->eat);
	if (ft_usleep(info->time_to_eat, info) == -1)
		return (unlock_forks(info, philo, 2));
	pthread_mutex_lock(&info->last_meal);
	philo->last_meal = ft_get_time();
	pthread_mutex_unlock(&info->last_meal);
	pthread_mutex_unlock(&(info->fork[philo->fork_l]));
	pthread_mutex_unlock(&(info->fork[philo->fork_r]));
}

void	*routine(void *arg)
{
	t_philo	*philo;
	t_info	*info;

	philo = (t_philo *)arg;
	info = philo->info;
	if (philo->id % 2)
		usleep(10000);
	while (1)
	{
		if (!actions(info, philo))
			break ;
		pthread_mutex_lock(&info->full_eat);
		if (info->is_full == 1)
		{
			pthread_mutex_unlock(&info->full_eat);
			break ;
		}
		pthread_mutex_unlock(&info->full_eat);
	}
	pthread_mutex_lock(&info->nb_thread_detached_mutex);
	info->nb_thread_detached += 1;
	pthread_mutex_unlock(&info->nb_thread_detached_mutex);
	return (0);
}

int	ft_detach_threads(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (pthread_detach(info->philo[i].thread) != 0)
			return (0);
		i++;
	}
	return (1);
}

int	start_philo(t_info *info)
{
	int		i;
	int		nb_thread_detached;
	t_philo	*philo;

	nb_thread_detached = 0;
	philo = info->philo;
	if (create_threads(info, philo) == 0)
		return (0);
	usleep(1000);
	check_death(info);
	pthread_mutex_lock(&info->nb_thread_detached_mutex);
	nb_thread_detached = info->nb_thread_detached;
	pthread_mutex_unlock(&info->nb_thread_detached_mutex);
	usleep(5000000);
	if (!ft_detach_threads(info))
		return (0);
	return (1);
}
