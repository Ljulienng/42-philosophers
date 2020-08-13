#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>

/*
** Returns the timestamp in milliseconds
*/

long    get_time(void)
{
    struct timeval  tp;
    long            milliseconds;

    gettimeofday(&tp, NULL);
    milliseconds = tp.tv_sec * 1000;
    milliseconds += tp.tv_usec / 1000;
    return (milliseconds);
}

void        ft_usleep(long int us)
{
    struct timeval start;
    struct timeval cur;
    
    gettimeofday(&cur, NULL);
	start = cur;
    while (((cur.tv_sec - start.tv_sec) * 1000000)
        + ((cur.tv_usec - start.tv_usec)) < us)
    {
        gettimeofday(&cur, NULL);   
        usleep(1);
    }
}

/*
** Prints time, sleeps 200ms, repeats!
*/

int main(void)
{
    long start_time;
		
		// Remember when we started
    start_time = get_time();

    while (1)
    {
				// Print time from start, in ms
        printf("%ld\n", get_time() - start_time);

				// Sleep 200 times 1000 microseconds (1 millisecond)
        ft_usleep(200 * 1000);
    }
}