/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauline <pauline@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:43:26 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/15 19:52:29 by pauline          ###   ########.fr       */
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

void			ft_usleep(long int us, t_philo *philo)
{
	struct timeval start;
	struct timeval now;

	gettimeofday(&now, NULL);
	start = now;
	while (((now.tv_sec - start.tv_sec) * 1000000)
		+ ((now.tv_usec - start.tv_usec)) < us)
	{
		if (((now.tv_usec / 1000) + now.tv_sec * 1000)
		- philo->diying > philo->set->time_to_die)
		{
			print_message(philo, DIED);
			return ;
		}
		gettimeofday(&now, NULL);
	}
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
	ft_usleep(philo->set->time_to_sleep * 1000, philo);
	if (!(print_message(philo, THINK)))
		return (0);
	return (1);
}

// void			*died2(void *arg)
// {
// 	t_philo *philo;

// 	philo = (void*)arg;
// 	while (philo->eating == 0)
// 	{
// 		if (get_time() - philo->diying > philo->set->time_to_die)
// 		{
// 			print_message(philo, DIED);
// 			return (NULL);
// 		}
// 	}
// 	return (NULL);
// }

void			*kill_program(t_philo *philo)
{
	sem_post(philo->set->lock);
	sem_post(philo->set->lock);
	return (NULL);
}

void			*start(void *arg)
{
	t_philo	*philo;

	philo = (t_philo*)arg;
	philo->diying = get_time();
	while (1)
	{
		// pthread_create(&philo->tid_died, NULL, &died2, philo);
		// pthread_detach(philo->tid_died);
		if (!(died(philo)))
			return (NULL);
		sem_wait(philo->set->lock);
		sem_wait(philo->set->lock);
		// philo->eating = 1;
		philo->set->ret -= 2;
		if (!(print_message(philo, EAT)))
			return (kill_program(philo));
		philo->diying = get_time();
		ft_usleep(philo->set->time_to_eat * 1000, philo);
		sem_post(philo->set->lock);
		sem_post(philo->set->lock);
		// philo->eating = 0;
		philo->set->ret += 2;
		philo->time_must_eat += 1;
		if ((philo->set->number_of_philosopher != -1 &&
		philo->time_must_eat ==
		philo->set->number_of_time_each_philosophers_must_eat)
		|| !(good_night(philo)))
			return (kill_program(philo));
	}
	return (NULL);
}
