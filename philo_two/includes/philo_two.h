/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo_one.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:14:25 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/02 18:44:33 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_ONE
# define PHILO_ONE

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
	sem_t	        *lock;
	sem_t	        *message;
	int				ret;
	struct timeval	tv;
	int				died;
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
	char			*nb;
	char			*time;
	int				state;
	int				nb_len;
	unsigned long	diying;
	int				time_must_eat;
	t_settings		*set;
}					t_philo;

int					ft_atoi(const char *s);
int					ft_strlen(const char *str);
char				*ft_itoa(long n);
unsigned long		get_time(void);
int					print_message(t_philo *philo, int str);

#endif
