/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 16:37:25 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/02 18:53:22 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_lock_eat_unlock(t_info *info, t_philo *philo)
{
	pthread_mutex_lock(&info->dead);
	if (info->is_dead == 1)
	{
		pthread_mutex_unlock(&info->dead);
		return (-1);
	}
	pthread_mutex_unlock(&info->dead);
	eat(philo, info);
	pthread_mutex_lock(&info->dead);
	if (info->is_dead == 1)
	{
		pthread_mutex_unlock(&info->dead);
		return (-1);
	}
	pthread_mutex_unlock(&info->dead);
	return (1);
}

long long	ft_get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

int	print_status(t_info *info, int id, char *status, int dead)
{
	long long	time;

	pthread_mutex_lock(&info->dead);
	if (info->is_dead == 1)
	{
		pthread_mutex_unlock(&info->dead);
		return (-1);
	}
	pthread_mutex_unlock(&info->dead);
	pthread_mutex_lock(&info->print);
	time = ft_get_time() - info->timestamp;
	pthread_mutex_lock(&info->dead);
	if (info->is_dead == 1)
	{
		pthread_mutex_unlock(&info->dead);
		pthread_mutex_unlock(&info->print);
		return (-1);
	}
	pthread_mutex_unlock(&info->dead);
	printf("%lld %d %s\n", time, id, status);
	if (dead == 0)
		pthread_mutex_unlock(&info->print);
	return (1);
}

int	ft_usleep(int time, t_info *info)
{
	long	start;

	start = ft_get_time();
	while (1)
	{
		pthread_mutex_lock(&info->dead);
		if (info->is_dead == 1)
		{
			pthread_mutex_unlock(&info->dead);
			return (-1);
		}
		pthread_mutex_unlock(&info->dead);
		if (ft_get_time() - start >= time)
			break ;
		usleep(50);
	}
	return (1);
}

int	create_threads(t_info *info, t_philo *philo)
{
	int	i;

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
	return (1);
}
