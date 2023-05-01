#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
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
	if(queue==NULL)
		return -1;
	else if(queue->length!=0)
		return -1;
	else {
		free(queue);
		return 0;
	}

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
	if(queue->length==0) // empty queue
		queue->head = node;
	else{
		queue->tail->next = node;
	}
	
	queue->tail = node;
	queue->length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL||data == NULL||queue->length==0)
		return -1;
	data = queue->head->info;
	queue->head = queue->head->next;
	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue==NULL || data==NULL)
		return -1;
	struct queue *previous, *current;
	// if head node matches data to be deleted
	if(queue->head->info==data) {
		current = queue->head;
		queue->head = queue->head->next;
		free(current); // delete current node
		queue->length--;
		return 0;
	}
	// move to node after head
	previous = queue->head;
	current = previous->next;
	while(current!=NULL && current->info != data){ // while no match found
		previous = current;
		current = current->next;
	}
	if(current!=NULL) {
		// match found
		previous->next = current->next;
		free(current);
		queue->length--;
		return 0;
	}
	return -1; // if data not found
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue==NULL || func==NULL)
		return -1;
	struct queue *current;
	current = queue->head; // initialize to head node
	while(current!=NULL){
		func(queue,current->info);
		current = current->next;
	}
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue==NULL) {
		return -1;
	}
	return queue->length;
}
