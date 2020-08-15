/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: pganglof <pganglof@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/09 20:02:41 by pganglof          #+#    #+#             */
/*   Updated: 2020/07/09 20:07:19 by pganglof         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_three.h"

void			waitmypid(pid_t *pid, int *status, t_settings *set)
{
	int		j;

	while ((*pid = waitpid(-1, status, 0)))
	{
		if (*pid == -1 && errno == ECHILD)
			break ;
		else if (*pid == -1)
			write(2, "\nwaitpid\n", 9);
		else if (WIFEXITED(*status))
		{
			if (WEXITSTATUS(*status) == 1)
			{
				j = 0;
				while (j < set->number_of_philosopher)
				{
					kill(set->all_child[j], SIGTERM);
					j++;
				}
				set->died = 1;
				return ;
			}
		}
	}
}

int				start_process(t_settings *set, t_philo *philo)
{
	int		i;
	int		status;
	pid_t	pid;

	i = 0;
	set->start_time = get_time();
	while (i < set->number_of_philosopher)
	{
		pid = fork();
		if (pid == 0)
			start(&(philo[i]));
		else if (pid > 0)
			set->all_child[i] = pid;
		else if (pid < 0)
			write(2, "\nCould not fork\n", 16);
		i++;
	}
	waitmypid(&pid, &status, set);
	return (1);
}

int				init_philosophers(t_settings *set)
{
	int			i;
	t_philo		*philo;

	i = 0;
	if (!(philo = malloc(sizeof(t_philo) * set->number_of_philosopher)))
		return (0);
	if (!(set->all_child = malloc(sizeof(pid_t) * set->number_of_philosopher)))
		return (0);
	while (i < set->number_of_philosopher)
	{
		memset(&philo[i], 0, sizeof(t_philo));
		philo[i].i = i + 1;
		philo[i].nb = ft_itoa(i + 1);
		philo[i].nb_len = ft_strlen(philo[i].nb);
		philo[i++].set = set;
	}
	if (!(start_process(set, philo)))
		return (0);
	if (set->died == 0)
		write(1, "All philosophers finished to eat\n", 33);
	i = 0;
	while (i < set->number_of_philosopher)
		free(philo[i++].nb);
	free(philo);
	free(set->all_child);
	return (1);
}

int				init_program(t_settings *set, int argc, char **argv)
{
	set->number_of_philosopher = ft_atoi(argv[1]);
	set->time_to_die = ft_atoi(argv[2]);
	set->time_to_eat = ft_atoi(argv[3]);
	set->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		set->number_of_time_each_philosophers_must_eat = ft_atoi(argv[5]);
	else
		set->number_of_time_each_philosophers_must_eat = -1;
	sem_unlink("semaphore");
	sem_unlink("message");
	if (!(set->lock =
		sem_open("semaphore", O_CREAT, S_IRWXG, set->number_of_philosopher)))
		return (0);
	if (!(set->message = sem_open("message", O_CREAT, S_IRWXG, 1)))
		return (0);
	init_philosophers(set);
	return (1);
}

int				main(int argc, char **argv)
{
	t_settings	set;

	if (argc != 5 && argc != 6)
		return (1);
	memset(&set, 0, sizeof(t_settings));
	if (!(init_program(&set, argc, argv)))
		return (1);
	sem_close(set.lock);
	sem_close(set.message);
	return (0);
}
