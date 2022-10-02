/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:04:53 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/02 16:15:01 by adlecler         ###   ########.fr       */
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
	long long		last_meal;
	int				nb_meals;
}		t_philo;

typedef struct s_info
{
	int	nb_philo;
	int	time_to_die;
	int	time_to_eat;
	int	time_to_sleep;
	int	nb_must_eat; // argument optionnel
	int	is_dead;
	int	is_full;
	int nb_thread_detached; //check
	int envoie_routine; //check
	int sortie_routine_normal; //check
	int sortie_routine_dead_full; //check
	pthread_mutex_t print;
	pthread_mutex_t fork[200];
	pthread_mutex_t dead;
	pthread_mutex_t last_meal;
	pthread_mutex_t eat;
	t_philo philo[200];
	long long timestamp; // timestamp de debut du programme 
}		t_info;

long		ft_atol(char *str);
int			ft_isdigit(char *s);
int			ft_check_overflow(char **av);
int			start_philo(t_info *info);
int 		check_meals(t_info *info, t_philo *philo);
int			check_death(t_info *info);
int			ft_lock_eat_unlock(t_info *info, t_philo *philo);
int			print_status(t_info *info, int id, char *status, int dead);
long long	ft_get_time(void);
int			ft_usleep(int time, t_info *info);
void		eat(t_philo *philo);

#endif

// quand un philo meurt : leaks
// when each pilo eat X time : conditionnal jump or move depends on uninitialised value(s)