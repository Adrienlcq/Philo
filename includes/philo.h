/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: adlecler <adlecler@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/09/05 14:04:53 by adlecler          #+#    #+#             */
/*   Updated: 2022/10/03 20:37:28 by adlecler         ###   ########.fr       */
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
	int				nb_meals;
	long long		last_meal;
	pthread_t		thread;
	struct s_info	*info;
}		t_philo;

typedef struct s_info
{
	int				nb_philo;
	int				time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				nb_must_eat;
	int				is_dead;
	int				is_full;
	int				nb_thread_detached;
	long long		timestamp;
	pthread_mutex_t	print;
	pthread_mutex_t	fork[210];
	pthread_mutex_t	dead;
	pthread_mutex_t	last_meal;
	pthread_mutex_t	eat;
	pthread_mutex_t	nb_thread_detached_mutex;
	pthread_mutex_t	full_eat;
	t_philo			philo[210];
}		t_info;

int			ft_isdigit(char *s);
int			ft_check_overflow(char **av);
int			start_philo(t_info *info);
int			check_meals(t_info *info, t_philo *philo);
int			check_death(t_info *info);
int			recover_info(char **av, t_info *info);
int			ft_lock_eat_unlock(t_info *info, t_philo *philo);
int			print_status(t_info *info, int id, char *status, int dead);
int			ft_usleep(int time, t_info *info);
int			philo_dead(int *check, int i, t_info *info);
void		eat(t_philo *philo, t_info *info);
long		ft_atol(char *str);
long long	ft_get_time(void);
int			check_nb_philo(int nb_philo);
int			create_threads(t_info *info, t_philo *philo);
int			philo_dead(int *check, int i, t_info *info);
void		unlock_forks(t_info *info, t_philo *philo, int nb);
void		*routine(void *arg);
int			actions(t_info *info, t_philo *philo);
int			init_mutex2(t_info *info);

#endif
