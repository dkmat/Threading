#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

// The semaphore struct is used to manage specific resources
struct semaphore { 
	int count;
	queue_t wait_queue;
};

// Initialize the amount of resources for this semaphore and return the created semaphore
sem_t sem_create(size_t count)
{
	sem_t create = malloc(sizeof(sem_t));
	if(create == NULL)
		return NULL;
	create->count = count;
	create->wait_queue = malloc(sizeof(queue_t));
	create->wait_queue = queue_create();
	return create;
}

// Deallocate the provided semaphore
int sem_destroy(sem_t sem)
{
	if(sem==NULL || queue_length(sem->wait_queue)>0)
		return -1;
	free(sem);
	return 0;
}

// Use the semaphore resource if available. If not available, then enqueue the current thread to the resource wait queue and block it.
int sem_down(sem_t sem)
{
	if(sem==NULL)
		return -1;
	if(sem->count>0)
		sem->count--;
	else{
		struct uthread_tcb * curr = uthread_current();
		queue_enqueue(sem->wait_queue,curr);
		uthread_block();
	}
	return 0;
}

// Free up the semaphore resource. If a thread is waiting, dequeue it from the wait queue and unblock it. 
int sem_up(sem_t sem)
{
	if(sem==NULL)
		return -1;
	sem->count++;
	if(queue_length(sem->wait_queue)>0){
		struct uthread_tcb * ready;
		queue_dequeue(sem->wait_queue,(void**)&ready);
		uthread_unblock(ready);
		sem->count--;
	}
	return 0;
}

