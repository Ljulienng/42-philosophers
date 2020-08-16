/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_two.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:14:25 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/16 02:35:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_TWO_H
# define PHILO_TWO_H

# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/time.h>

# define EAT 1
# define SLEEP 2
# define THINK 3
# define FORK 4
# define DIED 5

typedef struct		s_settings
{
	pthread_t		*tid;
	sem_t			*lock;
	sem_t			*message;
	sem_t			*sem_queue;
	int				ret;
	struct timeval	tv;
	int				died;
	unsigned long	start_time;
	int				number_of_philosopher;
	long			time_to_die;
	int				time_to_eat;
	int				time_to_sleep;
	int				number_of_time_each_philosophers_must_eat;
	int				number_of_philosophers_finished_to_eat;
}					t_settings;

typedef struct		s_philo
{
	pthread_t		tid_message;
	pthread_t		tid_died;
	char			*nb;
	char			*time;
	int				state;
	int				nb_len;
	long			diying;
	int				eating;
	int				time_must_eat;
	t_settings		*set;
}					t_philo;

int					ft_atoi(const char *s);
int					ft_strlen(const char *str);
char				*ft_itoa(long n);
unsigned long		get_time(void);
int					print_message(t_philo *philo, int str);
void				*start(void *arg);
void				ft_usleep(long int us, t_philo *philo);
void				*kill_program(t_philo *philo);

#endif
