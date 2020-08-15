/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/14 14:25:16 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/14 16:19:00 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*thread_eat(void *arg)
{
	t_philo *philo;

	philo = (t_philo*)arg;
	pthread_mutex_lock(&(philo->set->lock[philo->right]));
	pthread_mutex_lock(&(philo->set->lock[philo->left]));
	philo->eating = 1;
	return (NULL);
}

void	*start(void *arg)
{
	t_philo			*philo;

	philo = (t_philo*)arg;
	philo->set->start_time = get_time();
	philo->diying = get_time();
	while (philo->set->died == 0)
	{
		pthread_create(&(philo->tid_eat), NULL, &thread_eat, philo);
		pthread_detach(philo->tid_eat);
		while (philo->eating == 0)
		{
			if (get_time() - philo->diying > philo->set->time_to_die)
			{
				if (!(philo->time = ft_itoa(get_time()
				- philo->set->start_time)))
					return (NULL);
				print_message(philo, DIED);
				return (NULL);
			}
		}
		if (!(philosopher_meal(philo)))
			break ;
		if (!(philosopher_nap(philo)))
			break ;
	}
	return (NULL);
}

int		start_thread(t_settings *set, t_philo *philo)
{
	int		i;

	i = 0;
	while (i < set->number_of_philosopher)
	{
		if (pthread_create(&(set->tid[i]), NULL, &start, &(philo[i])) != 0)
			return (2);
		i += 2;
	}
	usleep(1);
	i = 1;
	while (i < set->number_of_philosopher)
	{
		if (pthread_create(&(set->tid[i]), NULL, &start, &(philo[i])) != 0)
			return (2);
		i += 2;
	}
	i = 0;
	while (i < set->number_of_philosopher)
		pthread_join(set->tid[i++], NULL);
	return (0);
}
