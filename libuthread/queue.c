#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

struct queue {
	struct queue* head;
	struct queue* tail;
	int length;

	void* info;
	struct queue* next;
};

queue_t queue_create(void)
{
	struct queue* start = malloc(sizeof(struct queue)); 
	if(start==NULL){
		printf("Error allocating memory for queue\n");
	}
	else start->length = 0;
	return start;
}

int queue_destroy(queue_t queue)
{
	if(queue!=NULL &&queue->length==0){
		free(queue);
		return 0;
	}
	return -1;
}

int queue_enqueue(queue_t queue, void *data)
{
	if(queue == NULL||data == NULL)
		return -1;
	struct queue* node = malloc(sizeof(struct queue));
	if(node == NULL)
		return -1;
	node->info = data;
	node->next = NULL;
	queue->tail->next = node;
	queue->tail = node;
	if(queue->length==0)
		queue->head = node;
	queue->length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL||data == NULL||queue->length==0)
		return -1;
	data = queue->head;
	queue->head = queue->head->next;
	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	/* TODO Phase 1 */
}

