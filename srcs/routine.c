/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/21 14:02:50 by adlecler          #+#    #+#             */
/*   Updated: 2022/09/21 15:51:15 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

int	ft_get_time(void)
{
	struct timeval time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000 + time.tv_usec / 1000));
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
	//philo->last_meal = ft_get_time();
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
	printf("JE suis philo numero: %d\n", philo->id + 1);
	if (philo->id % 2)
		usleep(15000);
	while (1)
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

	//Fonction pour checker la mort et le nombre de repas
	printf("Je suis avant le usleep\n");
	usleep(5000000); // A changer plus tard
	i = 0;
	while (i <= info->nb_philo)
		pthread_detach(philo[i].thread);
	return (1);
}