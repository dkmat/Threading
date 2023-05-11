#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "queue.h"

// This struct is to manage each item of our queue
struct node
{
	void *info;
	struct node *next;
};

// This struct is to manage our queue
struct queue
{
	struct node *head;
	struct node *tail;
	int length;
};

// Allocate memory and initialize the queue
queue_t queue_create(void)
{
	queue_t start = malloc(sizeof(queue_t));
	if (start == NULL)
		return NULL;
	start->length = 0;
	return start;
}

// Deallocate memory of the given queue
int queue_destroy(queue_t queue)
{
	if (queue == NULL || queue->length != 0)
		return -1;
	free(queue);
	return 0;
}

// Assign data to an item and add it to the given queue (items are added to the back of the queue, i.e., the tail)
int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL)
		return -1;
	struct node *item = malloc(sizeof(struct node));
	if (item == NULL)
		return -1;
	item->info = data;
	item->next = NULL;
	if (queue->length == 0) // empty queue
		queue->head = item;
	else
	{
		queue->tail->next = item;
	}
	queue->tail = item;
	queue->length++;
	return 0;
}

// Remove first item (i.e., oldest item) from the queue and return its info
int queue_dequeue(queue_t queue, void **data)
{
	if (queue == NULL || queue->length == 0)
		return -1;
	*data = queue->head->info;
	if (data == NULL)
		return -1;
	struct node *deall = queue->head;
	queue->head = queue->head->next;
	free(deall);
	queue->length--;
	return 0;
}
// Remove the first item from the queue that holds 'data' (the oldest item with 'data' is removed)
int queue_delete(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL || queue->length == 0)
		return -1;
	struct node *previous, *current;

	// if head node matches data to be deleted
	if (queue->head->info == data)
	{
		current = queue->head;	// re-configure pointers to prepare for deletion
		queue->head = queue->head->next;
		free(current); 			// delete current node
		queue->length--;
		return 0;
	}
	// move to node after head
	previous = queue->head;
	current = previous->next;
	// iterate through the rest of the queue until match is found
	while (current != NULL)
	{
		if (current->info == data)
		{									// if match found
			previous->next = current->next; // re-configure pointer to prepare for deletion
			free(current);
			queue->length--;
			return 0;
		}
		// move to next node
		previous = current;
		current = current->next;
	}
	return -1; // if data not found
}

// Goes through the whole queue and applies func to each item
int queue_iterate(queue_t queue, queue_func_t func)
{
	if (queue == NULL || func == NULL)
		return -1;
	struct node *current = queue->head; // initialize to head node
	struct node *keep;
	while (current != NULL)
	{
		keep = current->next; // preserve pointer in case we delete
		func(queue, current->info);
		current = keep;
	}
	return 0;
}

// Return the length of the given queue
int queue_length(queue_t queue) 
{
	if (queue == NULL)
		return -1;
	return queue->length;
}
