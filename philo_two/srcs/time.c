/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   time.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/16 02:31:43 by user42            #+#    #+#             */
/*   Updated: 2020/08/16 02:39:54 by user42           ###   ########.fr       */
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
	struct timeval	start;
	struct timeval	now;
	long			time;
	long			res;

	gettimeofday(&now, NULL);
	start = now;
	while (((now.tv_sec - start.tv_sec) * 1000000)
		+ ((now.tv_usec - start.tv_usec)) < us)
	{
		time = get_time();
		res = time - philo->diying;
		if (res > philo->set->time_to_die && res > 0)
		{
			print_message(philo, DIED);
			return ;
		}
		gettimeofday(&now, NULL);
	}
}

void			*kill_program(t_philo *philo)
{
	sem_post(philo->set->lock);
	sem_post(philo->set->lock);
	return (NULL);
}
