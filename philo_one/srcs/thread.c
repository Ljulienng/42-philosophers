/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pauline <pauline@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/14 14:25:16 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/15 20:07:33 by pauline          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

void	*died2(void *arg)
{
	long	now;
	int		res;
	t_philo *philo;

	philo = (t_philo*)arg;
	while (philo->set->fork[philo->right] == 0)
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

int		died_pair(t_philo *philo)
{
	long	now;
	int		res;

	while (philo->set->fork[philo->right] == 0
	|| philo->set->fork[philo->left] == 0)
	{
		now = get_time();
		res = now - philo->diying;
		if (res > philo->set->time_to_die && res > 0)
		{
			print_message(philo, DIED);
			return (0);
		}
	}
	philo->set->fork[philo->right] =
	pthread_mutex_lock(&(philo->set->lock[philo->right]));
	philo->set->fork[philo->left] =
	pthread_mutex_lock(&(philo->set->lock[philo->left]));
	return (1);
}

int		died(t_philo *philo)
{
	if (philo->set->number_of_philosopher % 2 == 1)
	{
		pthread_create(&philo->tid_eat, NULL, &died2, philo);
		pthread_detach(philo->tid_eat);
		if (philo->right % 2 == 0)
		{
			philo->set->fork[philo->right] =
			pthread_mutex_lock(&(philo->set->lock[philo->right]));
			philo->set->fork[philo->left] =
			pthread_mutex_lock(&(philo->set->lock[philo->left]));
		}
		else
		{
			philo->set->fork[philo->left] =
			pthread_mutex_lock(&(philo->set->lock[philo->left]));
			philo->set->fork[philo->right] =
			pthread_mutex_lock(&(philo->set->lock[philo->right]));
		}
	}
	else
	{
		if (!(died_pair(philo)))
			return (0);
	}
	return (1);
}

void	*start(void *arg)
{
	t_philo			*philo;

	philo = (t_philo*)arg;
	philo->diying = get_time();
	while (philo->set->died == 0)
	{
		if (!(died(philo)))
			return (NULL);
		if (!(philosopher_meal(philo)))
			return (NULL);
		if (!(philosopher_nap(philo)))
			return (NULL);
	}
	return (NULL);
}

int		start_thread(t_settings *set, t_philo *philo)
{
	int		i;

	i = 0;
	philo->set->start_time = get_time();
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
