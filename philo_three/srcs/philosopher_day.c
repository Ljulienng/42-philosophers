/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauline <pauline@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:03:22 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/15 19:51:33 by pauline          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

long			get_time(void)
{
	struct timeval	tv;
	long			ret;

	gettimeofday(&tv, NULL);
	ret = tv.tv_usec;
	ret /= 1000;
	ret += (tv.tv_sec * 1000);
	return (ret);
}

void			ft_usleep(long int us, t_philo *philo)
{
	struct timeval	start;
	struct timeval	now;
	long			time;
	int				res;

	gettimeofday(&now, NULL);
	start = now;
	while (((now.tv_sec - start.tv_sec) * 1000000)
		+ ((now.tv_usec - start.tv_usec)) < us)
	{
		time = get_time();
		res = time - philo->diying;
		if (res > philo->set->time_to_die && res > 0)
			print_message(philo, DIED);
		gettimeofday(&now, NULL);
	}
}

void			*died(void *arg)
{
	t_philo *philo;
	long	now;
	int		res;

	philo = (t_philo*)arg;
	while (philo->set->ret <= 1)
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
	philo->diying = get_time();
	while (1)
	{
		died(philo);
		sem_wait(philo->set->lock);
		sem_wait(philo->set->lock);
		philo->set->ret -= 2;
		print_message(philo, EAT);
		philo->diying = get_time();
		ft_usleep(philo->set->time_to_eat * 1000, philo);
		sem_post(philo->set->lock);
		sem_post(philo->set->lock);
		philo->set->ret += 2;
		philo->time_must_eat += 1;
		if (philo->set->number_of_philosopher != -1 &&
		philo->time_must_eat ==
		philo->set->number_of_time_each_philosophers_must_eat)
			exit(0);
		print_message(philo, SLEEP);
		ft_usleep(philo->set->time_to_sleep * 1000, philo);
		print_message(philo, THINK);
	}
}
