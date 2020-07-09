/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:03:22 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/09 21:05:08 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

unsigned long	get_time(void)
{
	struct timeval	tv;
	unsigned long	ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_usec;
	ret /= 1000;
	ret += (tv.tv_sec * 1000);
	return (ret);
}

void			*died(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo->ret == 0)
	{
		if (get_time() - philo->diying > philo->set->time_to_die)
		{
			print_message(philo, DIED);
			exit(1);
		}
	}
	return (NULL);
}

void			*eat(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	philo->ret = pthread_mutex_lock(&(philo->mutex_died));
	sem_wait(philo->set->lock);
	sem_wait(philo->set->lock);
	philo->ret = 1;
	print_message(philo, EAT);
	usleep(philo->set->time_to_eat * 1000);
	philo->time_must_eat += 1;
	sem_post(philo->set->lock);
	sem_post(philo->set->lock);
	pthread_mutex_unlock(&(philo->mutex_died));
	if (philo->set->number_of_philosopher != -1 &&
	philo->time_must_eat ==
	philo->set->number_of_time_each_philosophers_must_eat)
		exit(0);
	print_message(philo, SLEEP);
	usleep(philo->set->time_to_sleep * 1000);
	print_message(philo, THINK);
	return (NULL);
}

void			start(t_philo *philo)
{
	if (pthread_mutex_init(&(philo->mutex_died), NULL) != 0)
		exit(0);
	while (1)
	{
		philo->diying = get_time();
		if (pthread_create(&(philo->tid_died), NULL, &died, philo) != 0
			|| pthread_create(&(philo->tid_eat), NULL, &eat, philo) != 0)
		{
			write(2, "\nCan't create thread\n", 20);
			exit(0);
		}
		pthread_join(philo->tid_died, NULL);
		pthread_join(philo->tid_eat, NULL);
	}
	pthread_mutex_destroy(&(philo->mutex_died));
}
