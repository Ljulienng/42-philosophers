/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 18:30:29 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/02 18:41:47 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void			thread_print_two(t_philo *philo, unsigned long nbtime)
{
	write(1, philo->time, nbtime);
	write(1, " ms: ", 5);
	write(1, philo->nb, philo->nb_len);
	if (philo->state == EAT)
	{
		write(1, " has taken forks ", 17);
		write(1, philo->nb, philo->nb_len);
		write(1, " and ", 5);
		write(1, philo->s_left, philo->s_left_len);
		write(1, "\n", 1);
		write(1, philo->time, nbtime);
		write(1, " ms: ", 5);
		write(1, philo->nb, philo->nb_len);
		write(1, " is eating\n", 11);
	}
	else if (philo->state == SLEEP)
		write(1, " is sleeping\n", 13);
	else if (philo->state == THINK)
		write(1, " is thinking\n", 13);
	else
		write(1, " died\n", 6);
}

static void		*thread_print(void *arg)
{
	t_philo			*philo;
	unsigned long	nbtime;

	philo = (t_philo*)arg;
	// if (!philo->time)
	if (!(philo->time = ft_itoa(get_time() - philo->set->start_time)))
		return (NULL);
	nbtime = ft_strlen(philo->time);
	pthread_mutex_lock(&(philo->set->message));
	if (philo->set->died == 1)
		pthread_mutex_unlock(&(philo->set->message));
	else
	{
		if (philo->state == DIED)
			philo->set->died = 1;
		thread_print_two(philo, nbtime);
		pthread_mutex_unlock(&(philo->set->message));
	}
	free(philo->time);
	philo->time = NULL;
	return (NULL);
}

int				print_message(t_philo *philo, int str)
{
	philo->state = str;
	if (pthread_create(&(philo->tid_message), NULL, &thread_print, philo) != 0)
	{
		write(1, "\nCan't create thread\n", 20);
		return (0);
	}
	pthread_join(philo->tid_message, NULL);
	if (philo->set->died == 1)
		return (0);
	return (1);
}
