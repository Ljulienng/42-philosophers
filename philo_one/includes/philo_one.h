#ifndef PHILO_ONE
# define PHILO_ONE

# include <stdlib.h>
# include <pthread.h>
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/time.h>

typedef struct      s_settings
{
    pthread_mutex_t lock;
    struct timeval  tv;
    unsigned long   start_time;
    int             number_of_philosopher;
    int             time_to_die;
    int             time_to_eat;
    int             time_to_sleep;
    int             number_of_time_each_philosophers_must_eat;
}                   t_settings;

typedef struct      s_philo
{
    int             nb;
    int             dead;
    int             eating;
    int             sleeping;
    int             thinking;
    int             time_must_eat;
    t_settings      *set;
}                   t_philo;

int			        ft_atoi(const char *s);
int	    	        ft_isspace(char c);
int     		    ft_isdigit(int c);
int                 ft_strlen(const char *str);
char		        *ft_itoa(int n);
void			    ft_putstr(char const *s);

#endif
