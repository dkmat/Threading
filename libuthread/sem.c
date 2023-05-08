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
	/* TODO Phase 3 */
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
}

