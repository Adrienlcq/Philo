/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:04:53 by adlecler          #+#    #+#             */
/*   Updated: 2022/09/21 15:33:56 by adlecler         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H
# define MAX_INT 2147483647
# define MIN_INT -2147483648
# include <unistd.h>
# include <stdio.h>
# include <stddef.h>
# include <stdlib.h>
# include <string.h>
# include <pthread.h>
# include <sys/time.h>


typedef struct s_philo
{
	int				fork_r;
	int				fork_l;
	int				id;
	pthread_t		thread;
	struct s_info 	*info;
	int				last_meal;
	int				nb_meals;
}		t_philo;

typedef struct s_info
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_must_eat; // argument optionnel
	pthread_mutex_t fork[200];
	pthread_mutex_t dead;
	pthread_mutex_t print;
	pthread_mutex_t eat;
	t_philo philo[200];
	long long timestamp; // timestamp de debut du programme 
}		t_info;

long	ft_atol(char *str);
int		ft_isdigit(char *s);
int		ft_check_overflow(char **av);
int		start_philo(t_info *info);


#endif