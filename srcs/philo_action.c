/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_action.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/10/01 16:37:25 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/02 16:17:24 by adlecler         ###   ########.fr       */
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
	eat(philo);
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
	struct timeval time;

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
	printf("%lld %d %s\n", time, id + 1, status);
	//printf("%lld %d %s\n", time, id + 1, status);
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