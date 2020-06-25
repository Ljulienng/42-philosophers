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

void    *eat(void *arg)
{
    t_philo *philo;

    philo = (t_philo*)arg;
    pthread_mutex_lock(&(philo->set->lock));

    philo->eating = 1;
    printf("%ld %d is eating\n", get_time() - philo->set->start_time, philo->nb);

    usleep(philo->set->time_to_eat * 1000);

    pthread_mutex_unlock(&(philo->set->lock));
    return NULL;
}

void    thread(t_settings *set)
{
    int         i;
    t_philo     *philo;
    pthread_t   *tid;

    i = 0;
    tid = malloc(sizeof(pthread_t) * set->number_of_philosopher);
    philo = malloc(sizeof(t_philo) * set->number_of_philosopher);
    while (i < set->number_of_philosopher)
    {
        memset(&philo[i], 0, sizeof(t_philo));
        philo[i].nb = i + 1;
        philo[i].set = set;
        if (pthread_create(&(tid[i]), NULL, &eat, &(philo[i])) != 0)
        {
            printf("\ncan't create thread \n");
            return ;
        }
        i++;
    }
    i = 0;
    set->start_time = get_time();
    while (i < set->number_of_philosopher)
    {
        pthread_join(tid[i], NULL);
        i += 2;
    }
    i = 1;
    while (i < set->number_of_philosopher)
    {
        pthread_join(tid[i], NULL);
        i += 2;
    }
}

int     main(int argc, char **argv)
{
    t_settings  set;

    if (argc != 6)
        return (1);
    if (pthread_mutex_init(&(set.lock), NULL) != 0)
        return 1;
    set.number_of_philosopher = ft_atoi(argv[1]);
    set.time_to_die = ft_atoi(argv[2]);
    set.time_to_eat = ft_atoi(argv[3]);
    set.time_to_sleep = ft_atoi(argv[4]);
    set.number_of_time_each_philosophers_must_eat = ft_atoi(argv[5]);
    thread(&set);
    pthread_mutex_destroy(&(set.lock));
    return (0);
}
