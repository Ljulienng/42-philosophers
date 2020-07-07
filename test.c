# include <stdlib.h>
# include <pthread.h>
# include <semaphore.h>
# include <fcntl.h> 
# include <unistd.h>
# include <string.h>
# include <stdio.h>
# include <sys/stat.h>
# include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <signal.h>
int main() 
{ 
    for(int i=0;i<5;i++) // loop will run n times (n=5) 
    { 
        if(fork() == 0) 
        { 
            printf("[son] pid %d from [parent] pid %d\n",getpid(),getppid()); 
            exit(0); 
        } 
    } 
    for(int i=0;i<5;i++) // loop will run n times (n=5) 
    wait(NULL); 
      
} 