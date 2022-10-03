/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/11/25 16:38:08 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/02 18:48:48 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/philo.h"

long	ft_atol(char *str)
{
	int		i;
	long	result;
	int		negative;

	i = 0;
	negative = 1;
	result = 0;
	while ((str[i] == ' ') || (str[i] >= 9 && str[i] <= 13))
		i++;
	if (str[i] == '-' || str[i] == '+')
	{
		if (str[i] == '-')
			negative = negative * -1;
		i++;
	}
	while (str[i] >= '0' && str[i] <= '9')
	{
		result = (result * 10) + str[i] - 48;
		i++;
	}
	if (result < 0 && negative == 1)
		return (-1);
	else if (result < 0 && negative == -1)
		return (0);
	return (result * negative);
}

int	ft_isdigit(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (s[i] >= '0' && s[i] <= '9')
			i++;
		else
			return (0);
	}
	return (1);
}

int	ft_strlen(char *s)
{
	int	i;

	i = 0;
	while (s[i])
		i++;
	return (i);
}

int	ft_check_overflow(char **av)
{
	int		i;
	long	tmp;

	i = 0;
	while (av[++i])
	{
		tmp = ft_atol(av[i]);
		if (tmp < MIN_INT || tmp > MAX_INT)
		{
			printf("Error\nInvalid number (overflow)\n");
			return (0);
		}
		if (ft_strlen(av[i]) > 10)
		{
			printf("Error\nInvalid number (overflow)\n");
			return (0);
		}
	}
	return (1);
}

int	check_meals(t_info *info, t_philo *philo)
{
	int	i;

	i = 0;
	while (i < info->nb_philo)
	{
		if (philo[i].nb_meals < info->nb_must_eat)
			break ;
		i++;
	}
	if (i == info->nb_philo)
	{
		info->is_full = 1;
		return (1);
	}
	return (0);
}
