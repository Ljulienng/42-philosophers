/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_three.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:14:25 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/16 12:52:22 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_THREE_H
# define PHILO_THREE_H

# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/time.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <signal.h>
# include <errno.h>

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK 4
# define DIED 5

typedef struct		s_settings
{
	pid_t			*all_child;
	sem_t			*lock;
	sem_t			*message;
	sem_t			*sem_queue;
	pthread_mutex_t	queue;
	struct timeval	tv;
	int				died;
	int				ret;
	unsigned long	start_time;
	int				number_of_philosopher;
	long			time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				philosophers_must_eat;
	int				number_of_philosophers_finished_to_eat;
}					t_settings;

typedef struct		s_philo
{
	pthread_t		tid_message;
	pthread_t		tid_died;
	pthread_t		tid_eat;
	pthread_mutex_t	mutex_died;
	int				ret;
	int				i;
	char			*nb;
	char			*time;
	int				state;
	int				nb_len;
	unsigned long	diying;
	int				eating;
	int				time_must_eat;
	t_settings		*set;
}					t_philo;

long				ft_atoi(const char *s);
int					ft_strlen(const char *str);
char				*ft_itoa(long n);
long				get_time(void);
void				print_message(t_philo *philo, int str);
void				*eat(void *arg);
void				*died(void *arg);
void				start(t_philo *philo);

#endif
