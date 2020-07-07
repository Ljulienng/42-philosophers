#include "philo_three.h"

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

void    *start(t_philo *philo)
{
    while (1)
    {
        philo->diying = get_time();
        while (philo->set->ret <= 1)
        {
            if (get_time() - philo->diying > philo->set->time_to_die)
            {
                print_message(philo, DIED);
                return NULL;
            }
        }
        sem_wait(philo->set->lock);
        philo->set->ret -= 1;
        sem_wait(philo->set->lock);
        philo->set->ret -= 1;
        if (!(print_message(philo, EAT)))
        {
            sem_post(philo->set->lock);
                return NULL;
        }
        usleep(philo->set->time_to_eat * 1000);
        philo->time_must_eat += 1;

        sem_post(philo->set->lock);
        philo->set->ret += 1;
        sem_post(philo->set->lock);
        philo->set->ret += 1;

        if (philo->set->number_of_philosopher != -1 &&
        philo->time_must_eat ==
        philo->set->number_of_time_each_philosophers_must_eat)
                return NULL;

        if (!(print_message(philo, SLEEP)))
        {
            sem_post(philo->set->lock);
                return NULL;
        }
        usleep(philo->set->time_to_sleep * 1000);
        
        if (!(print_message(philo, THINK)))
        {
            sem_post(philo->set->lock);
                return NULL;
        }
    }
}

int		start_process(t_settings *set, t_philo *philo)
{
	int		i;
    // int     *status;
    // int     ret;
    int     pid;
	i = 0;
    (void)philo;
    // status = malloc(sizeof(int) * set->number_of_philosopher);
    // while (i < set->number_of_philosopher)
    //     set->pid[i++] = fork();
    i = 0;
	set->start_time = get_time();
    printf("number_of_philosopher : %d\n", set->number_of_philosopher);
    while (i < set->number_of_philosopher)
    {
        if ((pid = fork()) == 0)
        {
            
            // printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
            start(&(philo[i]));
            exit(0);
        }
    
        i++;
    }    
    i = 0;
    while (i < set->number_of_philosopher)
    {
        wait(NULL);
        // waitpid(set->pid[i], &(status[i]), 0);
        // ret = WEXITSTATUS(status[i]);

        //             // printf("i : %d\n", i);
        //     // printf("ret : %d\n", ret);
        //     if (ret == 1)
        //     {
        //         int j = 0;
        //             printf("j : %d\n", j);
        //         while (j < set->number_of_philosopher)
        //         {
        //             printf("j : %d\n", j);
        //             kill (set->pid[j++], 0);
        //         }
        //         exit (0);
        //     }
        i++;
    }


	return (1);
}

int		init_program(t_settings *set, int argc, char **argv)
{
	int		i;
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
    sem_unlink("semaphore");
    if (!(set->lock = sem_open("semaphore", O_CREAT, S_IRWXG, set->number_of_philosopher)))
        return(1);
    sem_unlink("message");
    if (!(set->message = sem_open("message", O_CREAT, S_IRWXG, 1)))
        return(1);
    i = 0;
	if (!(philo = malloc(sizeof(t_philo) * set->number_of_philosopher)))
		return (0);
	if (!(set->pid = malloc(sizeof(pid_t) * set->number_of_philosopher)))
		return (0);
	while (i < set->number_of_philosopher)
	{
		memset(&philo[i], 0, sizeof(t_philo));
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
	free(set->pid);
	free(philo);
	return (1);
}


int		main(int argc, char **argv)
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
