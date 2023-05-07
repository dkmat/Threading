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
	int state;
};

struct uthread_tcb *uthread_current(void)
{
	/* TODO Phase 2/3 */
	return NULL;
}

void uthread_yield(void)
{
	uthread_ctx_t * current;
	getcontext(current);
	uthread_ctx_t * next;
	queue_dequeue(ready_queue,(void**)&next);
	uthread_ctx_switch(current,next);
	queue_enqueue(ready_queue,current);
	if(queue_length(ready_queue)==1)
		uthread_exit();
}

void uthread_exit(void)
{
	uthread_ctx_t * current;
	getcontext(current);
	uthread_ctx_t * last;
	queue_dequeue(ready_queue,(void**)&last);
	uthread_ctx_switch(current,last);
	free(ready_queue);
}

int uthread_create(uthread_func_t func, void *arg)
{
	struct uthread_tcb *thread = malloc(sizeof(struct uthread_tcb));
	if(thread==NULL)
		return -1;
	thread->threadStack = uthread_ctx_alloc_stack(); // allocate stack
	if(thread->threadStack==NULL)
		return -1;
	int retval = uthread_ctx_init(thread->context, thread->threadStack, func, arg); // initialize context
	if(retval==-1)
		return -1;
	queue_enqueue(ready_queue,thread->context);
	thread->state = READY; //update state
	return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
	ready_queue = queue_create();
	struct uthread_tcb *idle = malloc(sizeof(struct uthread_tcb));
	idle->context = malloc(sizeof(uthread_ctx_t));
	idle->threadStack = uthread_ctx_alloc_stack();
	idle->state = RUNNING;
	if(!preempt)
		uthread_create(func,arg);
		uthread_yield();
	return 0;
}

void uthread_block(void)
{
	/* TODO Phase 3 */
}

void uthread_unblock(struct uthread_tcb *uthread)
{
	/* TODO Phase 3 */
	UNUSED(uthread);
}

