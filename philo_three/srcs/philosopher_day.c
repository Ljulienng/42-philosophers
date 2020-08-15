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

long	get_time(void)
{
	struct timeval	tv;
	long	ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_usec;
	ret /= 1000;
	ret += (tv.tv_sec * 1000);
	return (ret);
}

void			ft_usleep(long int us)
{
	struct timeval start;
	struct timeval now;

	gettimeofday(&now, NULL);
	start = now;
	while (((now.tv_sec - start.tv_sec) * 1000000)
		+ ((now.tv_usec - start.tv_usec)) < us)
		gettimeofday(&now, NULL);
}

void			*died(void *arg)
{
	t_philo *philo;
	long	now;
	int		res;

	philo = (t_philo*)arg;
	philo->diying = get_time();
	while (1)
	{
		now = get_time();
		res = now - philo->diying;
		if (res > philo->set->time_to_die && res > 0)
			print_message(philo, DIED);
	}
	return (NULL);
}

void			start(t_philo *philo)
{
	pthread_create(&(philo->tid_died), NULL, &died, philo);
	while (1)
	{
		sem_wait(philo->set->lock);
		sem_wait(philo->set->lock);
		philo->diying = get_time();
		print_message(philo, EAT);
		ft_usleep(philo->set->time_to_eat * 1000);
		sem_post(philo->set->lock);
		sem_post(philo->set->lock);
		philo->time_must_eat += 1;
		if (philo->set->number_of_philosopher != -1 &&
		philo->time_must_eat ==
		philo->set->number_of_time_each_philosophers_must_eat)
			exit(0);
		print_message(philo, SLEEP);
		ft_usleep(philo->set->time_to_sleep * 1000);
		print_message(philo, THINK);
	}
}
