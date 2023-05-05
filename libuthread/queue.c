#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

struct node{
	void* info;
	struct node* next;
};

struct queue {
	struct node* head;
	struct node* tail;
	int length;
};


queue_t queue_create(void)
{
	queue_t start = malloc(sizeof(queue_t)); 
	if(start==NULL){
		fprintf(stderr,"Error allocating memory for queue\n");
		return NULL;
	}
	start->length = 0;
	return start;
}

int queue_destroy(queue_t queue)
{
	if(queue==NULL||queue->length!=0)
		return -1;
	free(queue);
	return 0;
}

int queue_enqueue(queue_t queue, void *data)
{
	if(queue == NULL||data == NULL)
		return -1;
	struct node* item = malloc(sizeof(struct node));
	if(item == NULL)
		return -1;
	item->info = data;
	item->next = NULL;
	if(queue->length==0) // empty queue
		queue->head = item;
	else{
		queue->tail->next = item;

	}
	queue->tail = item;
	queue->length++;
	return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
	if(queue == NULL||queue->length==0)
		return -1;
	*data = queue->head->info;
	if(data==NULL)
		return -1;
	struct node* deall = queue->head;
	queue->head = queue->head->next;
	free(deall);
	queue->length--;
	return 0;
}

int queue_delete(queue_t queue, void *data)
{
	if(queue==NULL || data==NULL)
		return -1;
	struct node *previous, *current; 
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
	while(current!=NULL){ // while no match found
		if(current->info==data) {
		// match found
			previous->next = current->next;
			//int curr_info = *(int*)current->info;
			//fprintf(stderr, "current value: %d\n", curr_info);
			free(current);
			queue->length--;
			return 0;
		}
		previous = current;
		current = current->next;
	}
	return -1; // if data not found
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue==NULL || func==NULL)
		return -1;
	struct node *current = queue->head; // initialize to head node
	struct node *keep;
	while(current!=NULL){
		keep = current->next; // to preserve pointer in case we delete
		func(queue,current->info);
		current = keep;
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

