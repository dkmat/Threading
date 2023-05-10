#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

#define EXIT 	0
#define RUNNING 1
#define READY 	2
#define BLOCKED 3
#define UNUSED(x) (void)(x)

queue_t ready_queue;

struct uthread_tcb {
	void* threadStack;
	uthread_ctx_t* context;
};
struct uthread_tcb* current;

struct uthread_tcb *uthread_current(void)
{
	return current;
}

void uthread_yield(void)
{
	struct uthread_tcb* next;
	struct uthread_tcb* swap = current;
	queue_dequeue(ready_queue,(void**)&next); // dequeue next ready process
	queue_enqueue(ready_queue,current);// enqueue current process
	current = next;
	uthread_ctx_switch(swap->context,next->context); // context switch from current -> next
	
}

void uthread_exit(void)
{
	struct uthread_tcb* next;
	queue_dequeue(ready_queue,(void**)&next); // dequeue next ready process ()
	uthread_ctx_switch(current->context,next->context);// context switch from current -> next
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *thread = malloc(sizeof(struct uthread_tcb));
	if(thread==NULL)
		return -1;
	thread->threadStack = uthread_ctx_alloc_stack(); // allocate stack
	if(thread->threadStack==NULL)
		return -1;
	thread->context = malloc(sizeof(ucontext_t));
	int retval = uthread_ctx_init(thread->context, thread->threadStack, func, arg); // initialize context
	if(retval==-1) 
		return -1;
	queue_enqueue(ready_queue,thread);
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	ready_queue = queue_create();
	struct uthread_tcb *idle = malloc(sizeof(struct uthread_tcb));
	idle->context = malloc(sizeof(uthread_ctx_t));
	idle->threadStack = uthread_ctx_alloc_stack();
	if(!preempt){
		uthread_create(func,arg);
		while(queue_length(ready_queue)!=0){// while other threads are ready and waiting
			current = idle;
			uthread_yield();
		}
		fprintf(stderr,"back in main\n");
		queue_destroy(ready_queue);
	}
	return 0;
}

void uthread_block(void)
{
	struct uthread_tcb* next;
	struct uthread_tcb* swap = current;
	queue_dequeue(ready_queue,(void**)&next);
	current = next;
	uthread_ctx_switch(swap->context,next->context);
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	queue_enqueue(ready_queue,uthread);
}

