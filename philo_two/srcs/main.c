/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:24:10 by pganglof          #+#    #+#             */
/*   Updated: 2020/08/14 16:11:13 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_two.h"

void	ret_function(t_settings *set, t_philo *philo, int ret)
{
	int		i;

	if (ret == 2)
		write(1, "\nCan't create thread\n", 20);
	if (set->died == 0)
		write(1, "All philosophers finished to eat\n", 33);
	i = 0;
	while (i < set->number_of_philosopher)
		free(philo[i++].nb);
	free(philo);
}

int			start_thread(t_settings *set, t_philo *philo)
{
	int		i;

	i = 0;
	set->start_time = get_time();
	while (i < set->number_of_philosopher)
	{
		if (pthread_create(&(set->tid[i]), NULL, &start, &(philo[i])) != 0)
			return (2);
		i += 1;
	}
	i = 0;
	while (i < set->number_of_philosopher)
		pthread_join(set->tid[i++], NULL);
	return (1);
}

int			init_philo(t_settings *set, t_philo *philo)
{
	int		i;
	int		ret;

	i = 0;
	if (!(set->tid = malloc(sizeof(pthread_t) * set->number_of_philosopher)))
		return (0);
	while (i < set->number_of_philosopher)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].nb = ft_itoa(i + 1);
		philo[i].nb_len = ft_strlen(philo[i].nb);
		philo[i++].set = set;
	}
	ret = start_thread(set, philo);
	ret_function(set, philo, ret);
	return (1);
}

int			init_program(t_settings *set, int argc, char **argv)
{
	t_philo		*philo;

	set->number_of_philosopher = ft_atoi(argv[1]);
	set->ret = set->number_of_philosopher;
	set->time_to_die = ft_atoi(argv[2]);
	set->time_to_eat = ft_atoi(argv[3]);
	set->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		set->number_of_time_each_philosophers_must_eat = ft_atoi(argv[5]);
	else
		set->number_of_time_each_philosophers_must_eat = -1;
	if (!(philo = malloc(sizeof(t_philo) * set->number_of_philosopher)))
		return (0);
	sem_unlink("semaphore");
	sem_unlink("message");
	if (!(set->lock =
		sem_open("semaphore", O_CREAT, S_IRWXU, set->number_of_philosopher))
		|| !(set->message = sem_open("message", O_CREAT, S_IRWXU, 1)))
		return (1);
	if (!(init_philo(set, philo)))
		return (0);
	return (1);
}

int			main(int argc, char **argv)
{
	t_settings	set;

	if (argc != 5 && argc != 6)
		return (1);
	memset(&set, 0, sizeof(t_settings));
	if (!(init_program(&set, argc, argv)))
		return (1);
	free(set.tid);
	sem_close(set.lock);
	sem_close(set.message);
	return (0);
}
