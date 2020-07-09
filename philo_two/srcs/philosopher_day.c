/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:43:26 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/09 20:45:59 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

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

int				died(t_philo *philo)
{
	while (philo->set->ret <= 1)
	{
		if (get_time() - philo->diying > philo->set->time_to_die)
		{
			print_message(philo, DIED);
			return (0);
		}
	}
	return (1);
}

int				good_night(t_philo *philo)
{
	if (!(print_message(philo, SLEEP)))
		return (0);
	usleep(philo->set->time_to_sleep * 1000);
	if (!(print_message(philo, THINK)))
		return (0);
	return (1);
}

void			*start(void *arg)
{
	t_philo	*philo;

	philo = (t_philo*)arg;
	while (1)
	{
		philo->diying = get_time();
		if (!(died(philo)))
			return (NULL);
		sem_wait(philo->set->lock);
		sem_wait(philo->set->lock);
		philo->set->ret -= 2;
		if (!(print_message(philo, EAT)))
			return (NULL);
		usleep(philo->set->time_to_eat * 1000);
		philo->time_must_eat += 1;
		sem_post(philo->set->lock);
		sem_post(philo->set->lock);
		philo->set->ret += 2;
		if ((philo->set->number_of_philosopher != -1 &&
		philo->time_must_eat ==
		philo->set->number_of_time_each_philosophers_must_eat)
		|| !(good_night(philo)))
			return (NULL);
	}
	return (NULL);
}
