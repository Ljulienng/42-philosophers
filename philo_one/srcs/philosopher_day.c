/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosopher_day.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:16:34 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/02 18:37:53 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_one.h"

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

void        ft_usleep(long int us)
{
    struct timeval start;
    struct timeval cur;
    
    gettimeofday(&cur, NULL);
	start = cur;
    while (((cur.tv_sec - start.tv_sec) * 1000000)
        + ((cur.tv_usec - start.tv_usec)) < us)
    {
        gettimeofday(&cur, NULL);   
        usleep(1);
    }
}

int				philosopher_nap(t_philo *philo)
{
	if (!(print_message(philo, SLEEP)))
		return (0);
	// unsigned long end = get_time() + (philo->set->time_to_sleep);
	
	ft_usleep(philo->set->time_to_sleep * 1000);
	// usleep((philo->set->time_to_sleep - 10 )* 1000);
	// while (1)
	// {
		// if (get_time() > end)
			// break ;
	// }
	if (!(print_message(philo, THINK)))
		return (0);
	return (1);
}

int				kill_program(t_philo *philo)
{
	pthread_mutex_unlock(&(philo->set->lock[philo->right]));
	pthread_mutex_unlock(&(philo->set->lock[philo->left]));
	return (0);
}

int				philosopher_meal(t_philo *philo)
{
	philo->set->fork[philo->right] =
	pthread_mutex_lock(&(philo->set->lock[philo->right]));
	philo->set->fork[philo->left] =
	pthread_mutex_lock(&(philo->set->lock[philo->left]));
	if (!(print_message(philo, EAT)))
		return (kill_program(philo));
	philo->diying = get_time();
	// unsigned long end = get_time() + (philo->set->time_to_eat);
	ft_usleep(philo->set->time_to_eat * 1000);
	// usleep((philo->set->time_to_eat - 10) * 1000);
	// while (1)
	// {
	// 	if (get_time() > end)
	// 		break ;
	// }
	pthread_mutex_unlock(&(philo->set->lock[philo->right]));
	pthread_mutex_unlock(&(philo->set->lock[philo->left]));
	philo->time_must_eat += 1;
	philo->set->fork[philo->right] = 1;
	philo->set->fork[philo->left] = 1;
	if (philo->set->number_of_philosopher != -1 &&
	philo->time_must_eat ==
	philo->set->number_of_time_each_philosophers_must_eat)
		return (0);
	return (1);
}
