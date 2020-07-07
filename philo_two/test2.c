# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h> 
# include <unistd.h>
# include <string.h>
# include <stdio.h>
#include <sys/stat.h>
# include <sys/time.h>
 #include <errno.h>
# define PHILO 5

sem_t       *sem;
pthread_t	tid[5];
int         count = 0;


void    *start(void *arg)
{
    int    nb;
    printf("1/ \n");
    nb = *(int*)arg;
    printf("nb : %d\n", nb);

    sem_wait(sem);

    printf("2/ \n");
    count++;
    printf("nb %d - count : %d\n", nb, count);
    sem_post(sem);
    printf("3/ \n");
    return NULL;
}

int     main(void)
{
    int     i;
    errno = 0;
    sem_unlink("sem");
    if (!(sem = sem_open("sem", O_CREAT, 0600, 1)))

    // sem = sem_open("sem", O_CREAT, S_IRUSR, PHILO);
    i = 0;
    while (i < PHILO)
	{
		if (pthread_create(&(tid[i]), NULL, &start, &i) != 0)
		{
			write(1, "\nCan't create thread\n", 20);
			return (0);
		}
		i++;
	}
    i = 0;
	while (i < PHILO)
	{
		pthread_join(tid[i], NULL);
		i++;
	}
    return 0;
}
