/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:02:50 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/02 16:16:21 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	check_death(t_info *info)
{
	int	i;
	t_philo *philo;
	int check;

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
			{
				check = 1;
				pthread_mutex_unlock(&info->last_meal);
				print_status(info, i, "died", 1);
				pthread_mutex_lock(&info->dead);
				info->is_dead = 1;
				pthread_mutex_unlock(&info->dead);
				return (0);
			}
			if (check == 0)
				pthread_mutex_unlock(&info->last_meal);
			usleep(200);
		}
	}
	return (1);
}

void	eat(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	pthread_mutex_lock(&(info->fork[philo->fork_l]));
	if (print_status(info, philo->id, "has taken a fork", 0) == -1)
	{
		pthread_mutex_unlock(&(info->fork[philo->fork_l]));
		return ;
	}
	if (info->nb_philo == 1)
	{
		ft_usleep(info->time_to_die + 5, info);
		pthread_mutex_unlock(&(info->fork[philo->fork_l]));
		return ;
	}
	pthread_mutex_lock(&(info->fork[philo->fork_r]));
	if (print_status(info, philo->id, "has taken a fork", 0) == -1)
	{
		pthread_mutex_unlock(&(info->fork[philo->fork_l]));
		pthread_mutex_unlock(&(info->fork[philo->fork_r]));
		return ;
	}
	if (print_status(info, philo->id, "is eating", 0) == -1)
	{
		pthread_mutex_unlock(&(info->fork[philo->fork_l]));
		pthread_mutex_unlock(&(info->fork[philo->fork_r]));
		return ;
	}
	pthread_mutex_lock(&info->eat);
	philo->nb_meals++;
	pthread_mutex_unlock(&info->eat);
	if (ft_usleep(info->time_to_eat, info) == -1)
	{
		pthread_mutex_unlock(&(info->fork[philo->fork_l]));
		pthread_mutex_unlock(&(info->fork[philo->fork_r]));
		return ;
	}
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
		usleep(50000);
	while (1)
	{
		if (ft_lock_eat_unlock(info, philo) == -1)
			break ;
		if (print_status(info, philo->id, "is sleeping", 0) == -1)
			break ;
		if (ft_usleep(info->time_to_sleep, info) == -1)
			break ;
		if (print_status(info, philo->id, "is thinking", 0) == -1)
			break ;
		if (info->is_full == 1)
			break ;
	}
	info->nb_thread_detached += 1;
	return (0);
}

int	ft_detach_threads(t_info *info)
{
	int	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (pthread_detach(info->philo[i].thread) != 0)
			return (1);
		i++;
	}
	return (0);
}

int	start_philo(t_info *info)
{
	int	i;
	t_philo *philo;

	philo = info->philo;
	i = info->nb_philo - 1;
	info->timestamp = ft_get_time();
	while (i >= 0)
	{
		philo->thread = 0;
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
		{
			printf("Error: thread creation failed\n");
			return (0);
		}
		pthread_mutex_lock(&info->last_meal);
		philo[i].last_meal = ft_get_time();
		pthread_mutex_unlock(&info->last_meal);
		i--;
	}
	usleep(1000);
	check_death(info);
	usleep(5000000);
	if (info->nb_thread_detached != info->nb_philo)
		pthread_mutex_unlock(&info->print);
	while (info->nb_thread_detached != info->nb_philo)
		usleep(1000);
	ft_detach_threads(info);
	return (1);
}
