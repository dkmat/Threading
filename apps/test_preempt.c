#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include <uthread.h>
void thread2(void *arg){
    (void)arg;
    while(1){
        printf("thread2\n");
    }
}
void thread1 (void *arg){
    (void)arg;
    uthread_create(thread2,NULL);
    while(1){
        printf("thread1\n");
    }
}
int main(void){
    uthread_run(true,thread1,NULL);
    return 0;
}