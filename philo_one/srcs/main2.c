#include "philo_one.h"

unsigned long   get_time()
{
    struct timeval  tv;
    unsigned long   ret;

    gettimeofday(&tv, NULL);
    ret = tv.tv_usec;
    ret /= 1000;
    ret += (tv.tv_sec * 1000);
    return ret;
}

int    print_message(t_philo *philo, char *time, char *str)
{
    // if (philo->set->died || philo->set->number_of_time_each_philosophers_must_eat == philo->set->number_of_philosopher)
    // {
    //     return (0);
    // }
    pthread_mutex_lock(&(philo->set->message));
    write(1, time, strlen(time));
    write(1, " ", 1);
    write(1, philo->str_nb, philo->nb_len);
    write(1, str, strlen(str));
    pthread_mutex_unlock(&(philo->set->message));
    return (1);
}


void    *start(void *arg)
{
    int     right;
    int     left;
    t_philo *philo;
    char    *time;

    philo = (t_philo*)arg;
    
    
    right = philo->nb;
    left = (philo->nb - 1 == -1) ? philo->set->number_of_philosopher - 1 : (philo->nb - 1);
    while (1)
    {
        // if (philo->set->died)
        // {
        //     return (NULL);
        // }
        while (philo->set->fork[right] || philo->set->fork[left])
        {
        if (get_time() - philo->diying > philo->set->time_to_die)
        {
            time = ft_itoa(get_time() - philo->set->start_time);
            pthread_mutex_lock(&(philo->set->message));
            if (!(print_message(philo, ft_itoa(get_time() - philo->set->start_time), " died\n")))
                return (NULL);
            // write(1, time, strlen(time));
            // write(1, " ", 1);
            // write(1, philo->str_nb, philo->nb_len);
            // write(1, " died\n", 6);
            philo->set->died = 1;
            return NULL;
        }
        }

        philo->set->fork[right] = 1;
        philo->set->fork[left] = 1;
        pthread_mutex_lock(&(philo->set->lock[right]));
        pthread_mutex_lock(&(philo->set->lock[left]));
        if (!(print_message(philo, ft_itoa(get_time() - philo->set->start_time), " has taken his forks\n")))
            return (NULL);
        if (!(print_message(philo, ft_itoa(get_time() - philo->set->start_time), " is eating\n")))
            return (NULL);
        usleep(philo->set->time_to_eat * 1000);
        pthread_mutex_unlock(&(philo->set->lock[right]));
        pthread_mutex_unlock(&(philo->set->lock[left]));
        philo->set->fork[right] = 0;
        philo->set->fork[left] = 0;
        if (philo->set->number_of_philosopher != -1)
        {
            philo->time_must_eat += 1;
            if (philo->time_must_eat == philo->set->number_of_time_each_philosophers_must_eat)
            {
                philo->set->number_of_philosophers_finished_to_eat += 1;
                pthread_mutex_lock(&(philo->set->message));
                write(1, "All philosophers finished to eat\n", 33);
                return NULL;
            }
        }
        if (!(print_message(philo, ft_itoa(get_time() - philo->set->start_time), " is sleeping\n")))
            return(NULL);
        usleep(philo->set->time_to_sleep * 1000);
        if (!(print_message(philo, ft_itoa(get_time() - philo->set->start_time), " is thinking\n")))
            return (NULL);
        philo->diying = get_time();
    }
    return (NULL);
}

void    thread(t_settings *set)
{
    int         i;
    t_philo     *philo;

    i = 0;
    if (!(philo = malloc(sizeof(t_philo) * set->number_of_philosopher)))
        return ;
    if (!(set->tid = malloc(sizeof(pthread_t) * set->number_of_philosopher)))
        return ;
    while (i < set->number_of_philosopher)
    {
        memset(&philo[i], 0, sizeof(t_philo));
        philo[i].nb = i;
        philo[i].str_nb = ft_itoa(i + 1);
        philo[i].nb_len = strlen(philo[i].str_nb);
        philo[i].set = set;
        i++;
    }
    i = 0;
    set->start_time = get_time();
    while (i < set->number_of_philosopher)
    {
        philo[i].diying = get_time();
        if (pthread_create(&(set->tid[i]), NULL, &start, &(philo[i])) != 0)
        {
            // pthread_mutex_lock(&(philo->set->message));
            write(1, "\nCan't create thread\n", 20);
            return ;
        }
        pthread_detach(set->tid[i]);
        i++;
    }
    // i = 0;
    // while (i < set->number_of_philosopher)
    // {
    //     // pthread_join(set->tid[i], NULL);
    //     i++;
    // }
    if (philo->set->number_of_philosophers_finished_to_eat == philo->set->number_of_philosopher)
    {
            pthread_mutex_lock(&(philo->set->message));
            write(1, "All philosophers finished to eat\n", 33);
    }
}

int     main(int argc, char **argv)
{
    t_settings  set;

    if (argc != 5 && argc != 6)
        return (1);
    int i = 0;
    memset(&set, 0, sizeof(t_settings));
    set.number_of_philosopher = ft_atoi(argv[1]);
    set.lock = malloc(sizeof(pthread_mutex_t) * set.number_of_philosopher);
    set.fork = malloc(sizeof(int) * set.number_of_philosopher);
    i = 0;
    while (i < set.number_of_philosopher)
    {
        set.fork[i] = 0;
        i++;
    }
    if (pthread_mutex_init(&set.message, NULL) != 0)
        return (1);
    while (i < set.number_of_philosopher)
    {
        if (pthread_mutex_init(&(set.lock[i]), NULL) != 0)
            return (1);
        i++;
    }
    set.time_to_die = ft_atoi(argv[2]);
    set.time_to_eat = ft_atoi(argv[3]);
    set.time_to_sleep = ft_atoi(argv[4]);
    if (argc == 6)
        set.number_of_time_each_philosophers_must_eat = ft_atoi(argv[5]);
    else
        set.number_of_time_each_philosophers_must_eat = -1;
    thread(&set);
    i = 0;
    while (i < set.number_of_philosopher)
    {
        pthread_mutex_destroy(&(set.lock[i]));
        i++;
    }
    pthread_mutex_destroy(&(set.message));
    return (0);
}
