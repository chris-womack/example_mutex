/* Example code for starting
 * 2 threads and synchronizing  
 * their operation using a mutex.
 *
 * Author: Chris Womack
*/

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>
#include <util/util.h>
#include <sys/time.h>
#include <unistd.h>

pthread_t thread1;
pthread_t thread2;
pthread_mutex_t crit_sec_mutex;

// shared resource
static unsigned int counter;

void thread1_main(void);
void thread2_main(void);
void counter_oper(int thread_num);

void sig_handler(int signum) {
    if (signum != SIGINT) {
        printf("Received invalid signum = %d in sig_handler()\n", signum);
        ASSERT(signum == SIGINT);
    }

    printf("Received SIGINT. Executing pthread cancel for thread 1 & 2\n");

    pthread_cancel(thread1);
    pthread_cancel(thread2);
}

int main(void) {
    pthread_attr_t attr;
    int status;
 
    signal(SIGINT, sig_handler);

    counter = 0;

    pthread_attr_init(&attr);
    pthread_attr_setstacksize(&attr, 1024*1024);
   
    printf("Creating thread1\n");
    status = pthread_create(&thread1, &attr, (void*)&thread1_main, NULL);
    if (status != 0) {
        printf("Failed to create thread1 with status = %d\n", status);
        ASSERT(status == 0);
    }    

    printf("Creating thread2\n");
    status = pthread_create(&thread2, &attr, (void*)&thread2_main, NULL);
    if (status != 0) {
        printf("Failed to create thread2 with status = %d\n", status);
        ASSERT(status == 0);
    }    

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);
   
    printf("Returned to main after pthread_join for thread 1 & 2\n");
    return 0;
}

void thread1_main(void) {
    unsigned int exec_period_usecs;
    exec_period_usecs = 1000000; /*in micro-seconds*/

    printf("Thread 1 started. Execution period = %d uSecs\n", exec_period_usecs);
    while(1) {
        usleep(exec_period_usecs);
        counter_oper(1);
    }
}


void thread2_main(void) {
    unsigned int exec_period_usecs;
    exec_period_usecs = 1000000; /*in micro-seconds*/
    
    printf("Thread 2 started. Execution period = %d uSecs\n", exec_period_usecs);
    while(1) {
        usleep(exec_period_usecs);
        counter_oper(2);
    }
}

void counter_oper(int thread_num) {
    int i;
    struct timeval ts;

    // critical section
    // only one thread can execute this portion at a time
    pthread_mutex_lock(&crit_sec_mutex); // lock critical section
    gettimeofday(&ts, NULL); // move into loop to create deadlock
    printf("\n");
 
    for (i=0; i<10; i++) {
        counter += 1; // shared resource 
        printf("%06ld.%06ld: Thread %d, counter =  %d\n", ts.tv_sec, ts.tv_usec, thread_num, counter);
        //usleep(1000000);
    }

    printf("\n"); 
    pthread_mutex_unlock(&crit_sec_mutex); // unlock critical section
}

