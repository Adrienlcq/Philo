/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:02:50 by adlecler          #+#    #+#             */
/*   Updated: 2022/09/22 15:09:25 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000 + time.tv_usec / 1000));
}

int	check_death(t_info *info)
{
	int	i;

	while (1)
	{
		i = -1;
		while (++i < info->nb_philo)
		{
			if (ft_get_time() - info->philo[i].last_meal > info->time_to_die)
			{
				//pthread_mutex_lock(&info->print);
				info->is_dead = 1;
				printf("%lld %d died\n", ft_get_time() - info->timestamp, i + 1);
				//pthread_mutex_unlock(&info->dead);
				return (0);
			}
		}
	}
	return (1);
}

void	print_status(t_philo *philo, char *status)
{
	long long	time;

	pthread_mutex_lock(&philo->info->print);
	time = ft_get_time() - philo->info->timestamp;
	printf("%lld %d %s\n", time, philo->id + 1, status);
	pthread_mutex_unlock(&philo->info->print);
}


void	ft_usleep(int time)
{
	long	start;

	start = ft_get_time();
	while (ft_get_time() - start < time)
		usleep(100);
}

void	eat(t_philo *philo)
{
	t_info *info;

	info = philo->info;
	pthread_mutex_lock(&info->fork[philo->fork_l]);
	print_status(philo, "has taken a fork");
	pthread_mutex_lock(&info->fork[philo->fork_r]);
	print_status(philo, "has taken a fork");
	philo->last_meal = ft_get_time();
	print_status(philo, "is eating");
	//philo->nb_meals++;
	ft_usleep(info->time_to_eat);
	pthread_mutex_unlock(&info->fork[philo->fork_l]);
	pthread_mutex_unlock(&info->fork[philo->fork_r]);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	if (philo->id % 2) // pour opti et eviter les morts precoces des philos
		usleep(15000);
	while (philo->info->is_dead == 0)
	{
		eat(philo);
		print_status(philo, "is sleeping");
		ft_usleep(philo->info->time_to_sleep);
		print_status(philo, "is thinking");
	}
	return (NULL);
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
		if (pthread_create(&philo[i].thread, NULL, &routine, &philo[i]) != 0)
		{
			printf("Error: thread creation failed\n");
			return (0);
		}
		i++;
	}
	usleep(1000);
	check_death(info);
	//Fonction pour checker la mort et le nombre de repas
	usleep(5000000); // A changer plus tard
	printf("JE SUIS LA\n");
	//pthread_mutex_unlock(&info->print);
	i = 0;
	while (i <= info->nb_philo)
		pthread_detach(philo[i].thread);
	return (1);
}
