/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:14:25 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/14 14:25:44 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE_H
# define PHILO_ONE_H

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK 4
# define DIED 5

typedef struct		s_settings
{
	pthread_t		*tid;
	pthread_mutex_t	*lock;
	pthread_mutex_t	message;
	int				mes;
	int				died;
	struct timeval	tv;
	int				*fork;
	unsigned long	start_time;
	int				number_of_philosopher;
	unsigned int	time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_time_each_philosophers_must_eat;
	int				number_of_philosophers_finished_to_eat;
}					t_settings;

typedef struct		s_philo
{
	pthread_t		tid_message;
	pthread_t		tid_eat;
	int				right;
	int				left;
	char			*s_left;
	int				s_left_len;
	char			*nb;
	char			*time;
	int				state;
	int				nb_len;
	unsigned long	diying;
	int				eating;
	int				time_must_eat;
	t_settings		*set;
}					t_philo;

int					ft_atoi(const char *s);
int					ft_strlen(const char *str);
char				*ft_itoa(long n);
int					philosopher_meal(t_philo *philo);
int					philosopher_nap(t_philo *philo);
unsigned long		get_time(void);
int					print_message(t_philo *philo, int str);
int					kill_program(t_philo *philo);
int					start_thread(t_settings *set, t_philo *philo);
#endif
