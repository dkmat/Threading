#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

struct queue {
	struct node* head;
	struct node* tail;
	int length;
};

struct node{
	void* info;
	struct node* next;
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
	struct node *destroy = queue->head;
	while(destroy != NULL){
		struct node *next = destroy->next;
		free(destroy);
		destroy = next;
	}
	free(queue->tail);
	free(queue);
	queue = NULL;
	if(queue==NULL) {
		fprintf(stderr, "queue NULL\n");
	} else {
		fprintf(stderr, "queue NOT NULL\n");
	}
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
	struct node* deal = queue->head;
	queue->head = queue->head->next;
	free(deal);
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
	while(current!=NULL && current->info != data){ // while no match found
		previous = current;
		current = current->next;
	}
	if(current!=NULL) {
		// match found
		previous->next = current->next;
		int next_info = *(int*)previous->next->info;
		int curr_next_info = *(int*)current->next->info;
		fprintf(stderr, "inside delete(). curr->next->info = %d, previous->next->info = %d\n", curr_next_info, next_info);
		free(current);
		fprintf(stderr,"free correct\n");
		queue->length--;
		return 0;
	}
	return -1; // if data not found
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	if(queue==NULL || func==NULL)
		return -1;
	struct node *current;
	current = queue->head; // initialize to head node
	while(current!=NULL){
		fprintf(stderr,"here\n");
		int curr_info = *(int*)current->info;
		fprintf(stderr, "inside while loop. current->info = %d\n", curr_info);
		func(queue,current->info);
		fprintf(stderr,"finished func()\n");
		current = current->next;
	}
	fprintf(stderr, "outside while loop\n");
	return 0;
}

int queue_length(queue_t queue)
{
	if(queue==NULL) {
		return -1;
	}
	return queue->length;
}

