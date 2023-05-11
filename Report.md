# User-Level Thread Library
## Kimberly Tong, Daksh Mathur
## Summary
In this project we implement our own user-level thread library called
libuthread. We built the project from basics to complex. We went from queues to
threads to semaphores to preemption. We also built our own tests to test each
phase of our project. The tests we built aim to test how resistive our
implementation is.
## Queue Implementation
Understanding the requirement for us to have a FIFO(first in first out) queue
and time complexity O(1) for manipulating our queues except delete and iterate
we decided to use a linked list.
### Queue Structs
We decided to use two structs node and queue. The node struct holds the data and
a pointer to the next node. The queue struct has two node pointers pointing to
the front(head) and back(tail) of the queue and a variable to keep track of
queue length.
### Queue Functions
<h5 a><strong><code>queue_t queue_create(void)</code></strong></h5>
This function initializes the queue and returns the created queue.
<h5 a><strong><code>int queue_destroy(queue_t queue)</code></strong></h5>
This function checks if queue is empty and not NULL and then deallocates the
memory for the queue provided to the function. Returns 0 for success and -1 for
failure.
<h5 a><strong><code>int queue_enqueue(queue_t queue, void *data)</code></strong></h5>
This function checks if queue is not NULL and data is not NULL. If true then it
adds the node with data parameter to the queue. Every time an element is added
its added to the back of the queue unless it is the first element then it is the
front and back of the queue. Increments the variable keeping track of length.
Returns 0 for succes and -1 for failure.
<h5 a><strong><code>int queue_dequeue(queue_t queue, void **data)</code></strong></h5>
This function checks if queue is not NULL and data that is dequeued is not NULL
if true then it removes the element at the front of the queue because this is
the oldest item in the queue and gives it to data, also decrements the variable
keeping track of length. Returns 0 for success and -1 for failure.
<h5 a><strong><code>int queue_delete(queue_t queue, void *data)</code></strong></h5>
This function checks if queue is not NULL, data is not NULL and the queue is not
empty. If true then it deletes the oldest node with the data element in the
queue. Return 0 for success and -1 for failure.
<h5 a><strong><code>int queue_iterate(queue_t queue, queue_func_t func)</code></strong></h5>
This function checks if queue is not NULL and func parameter is not NULL if true
then it iterates through the entire queue and applies the function func to each
element. Return 0 for success and -1 for failure.
<h5 a><strong><code>int queue_length(queue_t queue)</code></strong></h5>
This function checks if queue is not NULL. Returns queue length for success and
-1 for failure.

## UThread Implementation
We use a struct to hold information about each thread's stack and its context.
We used the ucontext library functions to setup, execute and exit each thread.
We declared a global thread struct that to keep track of the current thread that
was executing. We also have a global queue called ready_queue to be able to
schedule threads based on when they are created.
### UThread Struct
To keep track of the ready threads, we use a queue of TCBs. Each TCB is
implemented as a uthread_tcb struct, which contains pointers to the stack and
context.
### UThread Functions
<h5 a><strong><code>void uthread_yield(void)</code></strong></h5>
This function allows the current thread to yield to the next ready thread in the
ready queue. It dequeues the next thread from the ready queue, enqueues the
current process, then context switches to the dequeued next thread.
<h5 a><strong><code>void uthread_exit(void)</code></strong></h5>
This function leaves the current thread and never returns to it that is why it
is called exit. This function is called when the thread is done executing and is
ready to become a zombie thread and be collected by the parent process.
<h5 a><strong><code>int uthread_create(uthread_func_t func, void *arg)</code></strong></h5>

This function creates a TCB (uthread_tcb struct) for a new thread. It does this
by allocating memory for the uthread_tcb struct, the stack (via
uthread_ctx_alloc_stack()), and the context, and initializing the context with
the provided function and arg (via uthread_ctx_init()). It then enqueues this to
the ready queue. 
<h5 a><strong><code>int uthread_run(void)</code></strong></h5>
This function acts as the main thread, this is always going to be the parent
thread. This function creates the other threads and executes them. This function
keeps running the threads until there are threads in the ready_queue. Once all
the threads are done executing and exit then this function returns 0 to exit.

# Semaphore Implementation
We understood from the instructions that when threads are running concurrently
and using the same resources then they could potentially change the same
resource and cause problems if we don't add proper measures for synchronization.
To avoid threads manipulating the same resources we implemented semaphores that
control resources and if two different threads try to access the same resource
one of them is put in the queue while the other uses the resource. The other one
can use the resource later when it becomes available.
### Semaphore Struct
To keep track of each semaphore's resource and its availability, we use a
semaphore struct, which contains the semaphore count (can be 0 or a positive
integer) and a wait queue, to keep track of the threads that are waiting to use
the resource.

### Semaphore Functions
<h5 a><strong><code>sem_t sem_create(size_t count)</code></strong></h5>
This function allocates memory for the semaphore and check if memory allocation
was successful. Set the internal count to the parameter count and allocates the
wait queue for the semaphore as well. Returns the created semaphore struct.
Returns -1 for failure.
<h5 a><strong><code>int sem_destroy(sem_t sem)</code></strong></h5>
This function checks if sem is not NULL and if the wait queue is empty if true
it deallocates the sem semaphore. Returns 0 for succes and -1 for failure. 
<h5 a><strong><code>int sem_down(sem_t sem)</code></strong></h5>
This function checks if sem is not NULL if true then it checks if semaphore is
available by checking the count. If it is available then it decrements it
because it is being used. Otherwise, it adds the current thread to the
semaphore's wait queue and dequeues it from the ready queue. This ensures the
thread cannot be scheduled until the semaphore becomes available. Returns 0 for
success and -1 for failure.
<h5 a><strong><code>int sem_up(sem_t sem)</code></strong></h5>
This function checks if sem is not NULL if true then it checks increases the count for the semaphore, then it checks if there are any threads in the wait queue and enqueues them to the ready queue. Since they are going to be using the resource we decrement the count again. Returns 0 for success and -1 for failure.
<h5 a><strong><code>void uthread_unblock(struct uthread_tcb *uthread)</code></strong></h5>
This function makes sure we switch out from the current thread to the next
thread in the ready queue. This ensures that we don't schedule the current
thread until it enters the ready queue again.
<h5 a><strong><code>void uthread_unblock(void)</code></strong></h5>
This function enqueues the thread freed from the wait queue into the ready
queue. This makes the uthread parameter ready and it can be scheduled in the
future.
