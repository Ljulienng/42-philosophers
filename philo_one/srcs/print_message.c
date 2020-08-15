/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_message.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauline <pauline@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 18:30:29 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/15 20:12:10 by pauline          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void			thread_print_two(t_philo *philo)
{
	unsigned long	nbtime;

	nbtime = ft_strlen(philo->time);
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

	philo = (t_philo*)arg;
	if (!(philo->time = ft_itoa(get_time() - philo->set->start_time)))
		return (NULL);
	pthread_mutex_lock(&(philo->set->message));
	if (philo->set->died == 1)
		pthread_mutex_unlock(&(philo->set->message));
	else
	{
		if (philo->state == DIED)
			philo->set->died = 1;
		thread_print_two(philo);
		pthread_mutex_unlock(&(philo->set->message));
	}
	free(philo->time);
	philo->time = NULL;
	return (NULL);
}

int				print_message(t_philo *philo, int str)
{
	philo->state = str;
	thread_print(philo);
	if (philo->set->died == 1)
		return (0);
	return (1);
}
