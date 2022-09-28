/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:02:50 by adlecler          #+#    #+#             */
/*   Updated: 2022/09/27 16:24:55 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long long	ft_get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	print_status(t_info *info, int id, char *status, int dead)
{
	long long	time;

	pthread_mutex_lock(&info->print);
	time = ft_get_time() - info->timestamp;
	printf("\033[30;01m%lld\033[00m \033[37;01m%d\033[00m %s\n", time, id + 1, status);
	if (dead == 0)
		pthread_mutex_unlock(&info->print);
}

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
			pthread_mutex_lock(&info->last_meal);
			if (ft_get_time() - philo[i].last_meal > info->time_to_die)
			{
				check = 1;
				pthread_mutex_unlock(&info->last_meal);
				print_status(info, i, "\033[31;01mdied\033[00m", 1);
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

void	ft_usleep(int time, t_info *info)
{
	long	start;

	start = ft_get_time();
	while (1)
	{
		pthread_mutex_lock(&info->dead);
		if (info->is_dead == 1)
			break ;
		pthread_mutex_unlock(&info->dead);
		if (ft_get_time() - start >= time)
			break ;	
		usleep(50);
	}
}

void	eat(t_philo *philo)
{
	t_info	*info;

	info = philo->info;
	
	pthread_mutex_lock(&(info->fork[philo->fork_l]));
	print_status(info, philo->id, "\033[34;01mhas taken a fork\033[00m", 0);
	pthread_mutex_lock(&(info->fork[philo->fork_r]));
	print_status(info, philo->id, "\033[34;01mhas taken a fork\033[00m", 0);
	print_status(info, philo->id, "\033[32;01mis eating\033[00m", 0);
	pthread_mutex_lock(&info->eat);
	philo->nb_meals++;
	pthread_mutex_unlock(&info->eat);
	ft_usleep(info->time_to_eat, info);
	pthread_mutex_lock(&info->last_meal);
	philo->last_meal = ft_get_time();
	pthread_mutex_unlock(&info->last_meal);
	pthread_mutex_unlock(&(info->fork[philo->fork_l]));
	pthread_mutex_unlock(&(info->fork[philo->fork_r]));
	printf("Nombre de repas : %d\n", philo->nb_meals);
	printf("Nombre de repas du dernier philo : %d\n", info->philo[info->nb_philo - 1].nb_meals);
}

int	ft_lock_eat_unlock(t_info *info, t_philo *philo)
{
	if (info->is_full == 1) //ajout
		return (-1);
	pthread_mutex_lock(&info->dead);
	if (info->is_dead == 1)
	{
		pthread_mutex_unlock(&info->dead);
		return (-1);
	}
	pthread_mutex_unlock(&info->dead);
	eat(philo);
	if (info->is_full == 1) //ajout
		return (-1);
	pthread_mutex_lock(&info->dead);
	if (info->is_dead == 1)
	{
		pthread_mutex_unlock(&info->dead);
		return (-1);
	}
	pthread_mutex_unlock(&info->dead);
	return (1);
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
		if (ft_lock_eat_unlock(info, philo) == -1)
			break;
		print_status(info, philo->id, "\033[36;01mis sleeping\033[00m", 0);
		ft_usleep(info->time_to_sleep, info);
		pthread_mutex_lock(&info->dead);
		if (info->is_dead == 1)
		{
			pthread_mutex_unlock(&info->dead);
			break;
		}
		if (info->nb_must_eat >= 0)
			check_meals(info, philo);
		if (info->is_full == 1)
			break;
		pthread_mutex_unlock(&info->eat);
		pthread_mutex_unlock(&info->dead);
		print_status(info, philo->id, "\033[35;01mis thinking\033[00m", 0);
	}
	return (0);
}

int	start_philo(t_info *info)
{
	int	i;
	t_philo *philo;

	philo = info->philo;
	i = 0;
	info->timestamp = ft_get_time();
	while (i < info->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, routine, &philo[i]) != 0)
		{
			printf("Error: thread creation failed\n");
			return (0);
		}
		pthread_mutex_lock(&info->last_meal);
		philo[i].last_meal = ft_get_time();
		pthread_mutex_unlock(&info->last_meal);
		i++;
	}
	printf("COUCOU\n");
	usleep(1000);
	printf("Valeur de is_full : %d\n", info->is_full);
	if (info->is_full == 1)
	{
		printf("Tous les philosophes ont mangé au moins %d fois\n", info->nb_must_eat);
		return (0);
	}
	if (check_death(info) == 0)
	{
		usleep(10000);
		return (0);
	}
	usleep(5000000);
	i = 0;
	while (i <= info->nb_philo)
		pthread_detach(philo[i++].thread);
	return (1);
}
