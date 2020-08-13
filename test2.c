# include <sys/time.h>
# include <unistd.h>
​
typedef struct timeval	t_time;
​
t_time g_starttime;
​
void					ft_putnbr(ssize_t n)
{
	char		c;
	ssize_t		signal;

	signal = 1;
	if (n < 0)
	{
		signal = -1;
		write(1, "-", 1);
	}
	if (n / 10)
		ft_putnbr((n / 10) * signal);
	c = ((unsigned int)(n * signal) % 10) + '0';
	write(1, &c, 1);
}
​
void					ft_putstr_fd(char *str, int fd)
{
	int i;

	i = 0;
	while (str[i])
	{
		write(fd, &str[i], 1);
		i++;
	}
}
​
unsigned long int		get_time()
{
	t_time now;
	gettimeofday(&now, NULL);
	return (((now.tv_sec * 1000000 + now.tv_usec)
		- (g_starttime.tv_sec * 1000000 + g_starttime.tv_usec)) / 1000);
}
​
void        ft_usleep(long int us)
{
    t_time start;
    t_time cur;
    
    gettimeofday(&cur, NULL);
	start = cur;
    while (((cur.tv_sec - start.tv_sec) * 1000000)
        + ((cur.tv_usec - start.tv_usec)) < us)
    {
        gettimeofday(&cur, NULL);   
        usleep(1);
    }
}
​
int main(void)
{
	gettimeofday(&g_starttime, NULL);
	while (1)
	{
		ft_putnbr(get_time());
		ft_putstr_fd("\n", 1);
		ft_usleep(100 * 1000);
	}
	return (0);
}