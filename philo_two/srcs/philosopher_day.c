/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:43:26 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/16 02:39:49 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

int				died_even(t_philo *philo)
{
	long	time;
	long	res;

	while (philo->set->ret <= 1)
	{
		time = get_time();
		res = time - philo->diying;
		if (res > philo->set->time_to_die && res > 0)
		{
			print_message(philo, DIED);
			return (0);
		}
	}
	return (1);
}

void			*died_odd(void *arg)
{
	t_philo				*philo;
	unsigned long		now;
	long				res;

	philo = (t_philo*)arg;
	while (philo->eating == 0)
	{
		now = get_time();
		res = now - philo->diying;
		if (res > philo->set->time_to_die && res > 0)
		{
			print_message(philo, DIED);
			return (NULL);
		}
	}
	return (NULL);
}

int				good_night(t_philo *philo)
{
	if (!(print_message(philo, SLEEP)))
		return (0);
	ft_usleep(philo->set->time_to_sleep * 1000, philo);
	if (!(print_message(philo, THINK)))
		return (0);
	return (1);
}

int				eat(t_philo *philo)
{
	philo->eating = 1;
	philo->diying = get_time();
	if (philo->set->number_of_philosopher % 2 == 1)
		sem_post(philo->set->sem_queue);
	philo->set->ret -= 2;
	if (!(print_message(philo, EAT)))
		return (0);
	ft_usleep(philo->set->time_to_eat * 1000, philo);
	sem_post(philo->set->lock);
	sem_post(philo->set->lock);
	philo->eating = 0;
	philo->set->ret += 2;
	philo->time_must_eat += 1;
	if (philo->set->number_of_philosopher != -1 &&
		philo->time_must_eat ==
		philo->set->number_of_time_each_philosophers_must_eat)
		return (0);
	return (1);
}

void			*start(void *arg)
{
	t_philo	*philo;

	philo = (t_philo*)arg;
	philo->diying = get_time();
	while (1)
	{
		if (philo->set->number_of_philosopher % 2 == 0)
		{
			if (!(died_even(philo)))
				return (NULL);
		}
		else
		{
			pthread_create(&philo->tid_died, NULL, &died_odd, philo);
			pthread_detach(philo->tid_died);
			if (philo->set->number_of_philosopher % 2 == 1)
				sem_wait(philo->set->sem_queue);
		}
		sem_wait(philo->set->lock);
		sem_wait(philo->set->lock);
		if (!(eat(philo)) || !(good_night(philo)))
			return (kill_program(philo));
	}
	return (NULL);
}
