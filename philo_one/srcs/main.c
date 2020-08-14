/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/02 16:14:15 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/09 20:49:45 by pganglof         ###   ########.fr       */
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
	philo->diying = get_time();
	while (philo->set->died == 0)
	{
		pthread_create(&(philo->tid_eat), NULL, &thread_eat, philo);
		while (philo->eating == 0)
		{
			if (get_time() - philo->diying > philo->set->time_to_die)
			{
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
	set->start_time = get_time();
	while (i < set->number_of_philosopher)
	{
		if (pthread_create(&(set->tid[i]), NULL, &start, &(philo[i])) != 0)
		{
			write(1, "\nCan't create thread\n", 20);
			return (0);
		}
		i += 2;
	}
	usleep(1);
	i = 1;
	while (i < set->number_of_philosopher)
	{
		if (pthread_create(&(set->tid[i]), NULL, &start, &(philo[i])) != 0)
		{
			write(1, "\nCan't create thread\n", 20);
			return (0);
		}
		i += 2;
	}	
	i = 0;
	while (i < set->number_of_philosopher)
	{
		pthread_join(set->tid[i], NULL);
		i++;
	}
	if (set->died == 0)
		write(1, "All philosophers finished to eat\n", 33);
	i = 0;
	while (i < set->number_of_philosopher)
		free(philo[i++].nb);
	return (1);
}

int		thread(t_settings *set)
{
	int			i;
	t_philo		*philo;

	i = 0;
	if (!(philo = malloc(sizeof(t_philo) * set->number_of_philosopher))
		|| !(set->tid = malloc(sizeof(pthread_t) * set->number_of_philosopher)))
		return (0);
	while (i < set->number_of_philosopher)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].right = i;
		philo[i].left = (i + 1) % set->number_of_philosopher;
		philo[i].s_left = ft_itoa(philo[i].left + 1);
		philo[i].s_left_len = ft_strlen(philo[i].s_left);
		philo[i].nb = ft_itoa(i + 1);
		philo[i].nb_len = ft_strlen(philo[i].nb);
		philo[i].set = set;
		philo[i].set->fork[philo[i].right] = 1;
		philo[i].set->fork[philo[i].left] = 1;
		i++;
	}
	if (!(start_thread(set, philo)))
		return (0);
	i = 0;
	while (i < set->number_of_philosopher)
		free(philo[i++].s_left);
	free(philo);
	return (1);
}

int		init_program(t_settings *set, int argc, char **argv)
{
	int		i;

	set->number_of_philosopher = ft_atoi(argv[1]);
	if (!(set->lock = malloc(sizeof(pthread_mutex_t)
	* set->number_of_philosopher)))
		return (0);
	if (!(set->fork = malloc(sizeof(int) * set->number_of_philosopher)))
		return (0);
	memset(set->fork, 1, sizeof(int) * set->number_of_philosopher);
	i = 0;
	while (i < set->number_of_philosopher)
		if (pthread_mutex_init(&(set->lock[i++]), NULL) != 0)
			return (0);
	if (pthread_mutex_init(&set->message, NULL) != 0)
		return (0);
	set->time_to_die = ft_atoi(argv[2]);
	set->time_to_eat = ft_atoi(argv[3]);
	set->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		set->number_of_time_each_philosophers_must_eat = ft_atoi(argv[5]);
	else
		set->number_of_time_each_philosophers_must_eat = -1;
	return (1);
}

int		main(int argc, char **argv)
{
	t_settings	set;
	int			i;

	if (argc != 5 && argc != 6)
		return (1);
	memset(&set, 0, sizeof(t_settings));
	set.mes = 1;
	if (!(init_program(&set, argc, argv)))
		return (1);
	if (!(thread(&set)))
		return (1);
	free(set.tid);
	free(set.lock);
	free(set.fork);
	i = 0;
	while (i < set.number_of_philosopher)
		pthread_mutex_destroy(&(set.lock[i++]));
	pthread_mutex_destroy(&(set.message));
	return (0);
}
