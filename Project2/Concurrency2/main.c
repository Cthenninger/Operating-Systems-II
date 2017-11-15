#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>

#define NUM 5
pthread_t philosophers[NUM];
pthread_mutex_t forks[NUM];

char *philosophersnames[NUM] = {"Periyar",
                    "Kamaraj",
                    "Gaarder",
                    "Bharathi",
                    "Nietzsche"
                   };


//Do think action				   
void Do_Think(int index)
{
    //wait for 1-20 seconds
    int waittime = rand() % 20 + 1;
    printf("%s is thinking for %d seconds\n", philosophersnames[index], waittime);
    sleep(waittime);
}

//Do eat action
void Do_Eat(int index)
{	
    //wait for 2-9 seconds while eating
    int waittime = rand() % 7 + 2;
    printf("%s is eating for %d seconds\n", philosophersnames[index], waittime);
    sleep(waittime);
    printf("%s has finished eating\n", philosophersnames[index]);
}

//Do philsopher actions
void* philosopher(void* i)
{
    int index = *((int*)i);
    while(1){
        //think
		Do_Think(index);

		//grab forks based on if 
        if(index % 2){	
            //get fork
            pthread_mutex_lock(&forks[index]);
            //get next fork
            pthread_mutex_lock(&forks[(index + 1) % NUM]);
        } 
		
		else{
            //get next fork
            pthread_mutex_lock(&forks[(index + 1) % NUM]);
            //get fork
            pthread_mutex_lock(&forks[index]);
        }
		
        //eat
        Do_Eat(index);
        //give up fork
        pthread_mutex_unlock(&forks[(index + 1) % NUM]);
        //give up other fork
        pthread_mutex_unlock(&forks[index]);
    }
}

//initialize locks and threads and call invoke philosopher()
int main()
{
    int i = 0;
    //void* return_val;
	
    //initialize forks as locks
    for(i = 0;i < NUM;i++)
    {
        pthread_mutex_init(&forks[i], NULL);
    }
	
    //create philosophers as threads
    for(i = 0;i < NUM;i++)
    {
        pthread_create(&philosophers[i], NULL, &philosopher, &i);
    }
    //join the threads
    for(i = 0;i < NUM;i++)
    {
        pthread_join(philosophers[i], NULL);
    }

    return 0;
}



