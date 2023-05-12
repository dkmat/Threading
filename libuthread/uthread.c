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

queue_t ready_queue; // queue of threads that are ready to be scheduled

// This struct acts as a TCB, keeping track of the thread's stack pointer and context
struct uthread_tcb {
	void* threadStack;
	uthread_ctx_t* context;
};
struct uthread_tcb* current;

struct uthread_tcb *uthread_current(void)
{
	return current;
}

// Yield to next thread in the ready queue
void uthread_yield(void)
{
	preempt_disable();
	struct uthread_tcb* next;
	struct uthread_tcb* swap = current;
	queue_dequeue(ready_queue,(void**)&next);	// dequeue next ready thread
	queue_enqueue(ready_queue,current);			// enqueue current thread
	current = next;
	preempt_enable();
	uthread_ctx_switch(swap->context,next->context); // context switch from current -> next
}

// Exit from currently running thread
void uthread_exit(void)
{
	preempt_disable();
	struct uthread_tcb* next;
	queue_dequeue(ready_queue,(void**)&next); // dequeue next ready thread
	preempt_enable();
	uthread_ctx_switch(current->context,next->context); // context switch from current -> next
}

// Create and initialize a thread and enqueue it to the ready queue
int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *thread = malloc(sizeof(struct uthread_tcb));
	if(thread==NULL)
		return -1;
	thread->threadStack = uthread_ctx_alloc_stack(); // allocate stack
	if(thread->threadStack==NULL)
		return -1;
	thread->context = malloc(sizeof(ucontext_t)); // allocate context
	int retval = uthread_ctx_init(thread->context, thread->threadStack, func, arg); // initialize context
	if(retval==-1) 
		return -1;
	queue_enqueue(ready_queue,thread); // enqueue the created thread to the ready queue
	return 0;
}

// Start the multithreading scheduling library; idle until there are no other threads to be scheduled, return when all threads have finished executing
int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	ready_queue = queue_create(); // create the ready queue
	// Set up 'idle' thread
	struct uthread_tcb *idle = malloc(sizeof(struct uthread_tcb));
	idle->context = malloc(sizeof(uthread_ctx_t));
	idle->threadStack = uthread_ctx_alloc_stack();
	
	preempt_start(preempt);
	uthread_create(func,arg);
	if(!preempt){
		while(queue_length(ready_queue)!=0){ // while other threads are ready and waiting
			current = idle;
			uthread_yield();
		}
	}
	else{
		while(1){
			current = idle;
		}
	}
	queue_destroy(ready_queue);
	if(preempt)
		preempt_stop();
	return 0;
}

// Context switch from the current thread to the next thread in the ready queue
void uthread_block(void)
{
	struct uthread_tcb* next;
	struct uthread_tcb* swap = current;
	queue_dequeue(ready_queue,(void**)&next);
	current = next;
	uthread_ctx_switch(swap->context,next->context);
}

// Enqueue the provided thread to the ready queue
void uthread_unblock(struct uthread_tcb *uthread)
{
	queue_enqueue(ready_queue,uthread);
}

