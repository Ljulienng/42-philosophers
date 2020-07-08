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

void            *died(void *arg)
{
    t_philo *philo;

	philo = (t_philo*)arg;
    while (philo->ret == 0)
    {
        if (get_time() - philo->diying > philo->set->time_to_die)
        {
            print_message(philo, DIED);
            exit (1) ;
        }
    }
    return NULL;
}

void             *eat(void *arg)
{
    t_philo *philo;

	philo = (t_philo*)arg;

    philo->ret = pthread_mutex_lock(&(philo->mutex_died));

    sem_wait(philo->set->lock);
    sem_wait(philo->set->lock);

    philo->ret = 1;


    if (!(print_message(philo, EAT)))
    {
        sem_post(philo->set->lock);
        sem_post(philo->set->lock);
        exit (0);
    }
    usleep(philo->set->time_to_eat * 1000);
    philo->time_must_eat += 1;
    sem_post(philo->set->lock);
    sem_post(philo->set->lock);


    if (philo->set->number_of_philosopher != -1 &&
    philo->time_must_eat ==
    philo->set->number_of_time_each_philosophers_must_eat)
        exit (0);

    if (!(print_message(philo, SLEEP)))
    {
        sem_post(philo->set->lock);
        sem_post(philo->set->lock);
        exit (0);
    }
    usleep(philo->set->time_to_sleep * 1000);
    
    if (!(print_message(philo, THINK)))
    {
        sem_post(philo->set->lock);
        sem_post(philo->set->lock);
        exit (0);
    }
    return NULL;
}

void             start(t_philo *philo)
{
    if (pthread_mutex_init(&(philo->mutex_died), NULL) != 0)
        exit (0);
    while (1)
    {
        philo->diying = get_time();

        if (pthread_create(&(philo->tid_died), NULL, &died, philo) != 0)
        {
            write(1, "\nCan't create thread\n", 20);
		    exit (0);
        }
        if (pthread_create(&(philo->tid_eat), NULL, &eat, philo) != 0)
        {
            write(1, "\nCan't create thread\n", 20);
		    exit (0);
        }
    	pthread_join(philo->tid_died, NULL);
    	pthread_join(philo->tid_eat, NULL);

    }
        pthread_mutex_destroy(&(philo->mutex_died));
}

int             start_process(t_settings *set, t_philo *philo)
{
    int     i;
    int     status;
    pid_t   pid;
    pid_t   *all_child;

    if (!(all_child = malloc(sizeof(pid_t) * set->number_of_philosopher)))
        return (0);
    i = 0;
    set->start_time = get_time();
    while (i < set->number_of_philosopher)
    {
        pid = fork();
        if (pid == 0)
            start(&(philo[i]));
        else if (pid > 0)
            all_child[i] = pid;
        else if (pid < 0)
            write(2, "\nCould not fork\n", 16);
        i++;
    }
    while ((pid = waitpid(-1, &status, 0)))
    {
        if (pid == -1)
            write(2, "\nwaitpid\n", 8);
        else if (WIFEXITED(status))
        {
            if (WEXITSTATUS(status) == 1)
            {
                for (int j = 0; j < set->number_of_philosopher; j++)
                    kill(all_child[j], SIGKILL);
                set->died = 1;
                break ;
            }
        }
    }
    free(all_child);
    return (1);
}

int     init_philosophers(t_settings *set)
{
    int         i;
	t_philo		*philo;

    i = 0;
	if (!(philo = malloc(sizeof(t_philo) * set->number_of_philosopher)))
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
	free(philo);
    return (1);
}

int		init_program(t_settings *set, int argc, char **argv)
{
	set->number_of_philosopher = ft_atoi(argv[1]);
	set->time_to_die = ft_atoi(argv[2]);
	set->time_to_eat = ft_atoi(argv[3]);
	set->time_to_sleep = ft_atoi(argv[4]);
	if (argc == 6)
		set->number_of_time_each_philosophers_must_eat = ft_atoi(argv[5]);
	else
		set->number_of_time_each_philosophers_must_eat = -1;
    sem_unlink("/semaphore");
    sem_unlink("/message");
    if (!(set->lock = sem_open("/semaphore", O_CREAT, S_IRWXG, set->number_of_philosopher)))
        return (0);
    if (!(set->message = sem_open("/message", O_CREAT, S_IRWXG, 1)))
        return (0);
    init_philosophers(set);
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
