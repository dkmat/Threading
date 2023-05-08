#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	int count;
	queue_t wait_queue;
};

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

int sem_destroy(sem_t sem)
{
	if(sem==NULL || queue_length(sem->wait_queue)>0)
		return -1;
	free(sem);
	return 0;
}

int sem_down(sem_t sem)
{
	if(sem==NULL)
		return -1;
	if(sem->count>0)
		sem->count--;
	else{
		uthread_block();
		queue_enqueue(sem->wait_queue,uthread_current());
	}
	return 0;
}

int sem_up(sem_t sem)
{
	if(sem==NULL)
		return -1;
	sem->count++;
	if(queue_length(sem->wait_queue)>0){
		struct uthread_tcb * ready;
		queue_dequeue(sem->wait_queue,(void**)&ready);
		uthread_unblock(ready);
	}
	return 0;
}

