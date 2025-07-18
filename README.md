# User-Level Thread Library
## Kimberly Tong, Daksh Mathur
## Summary
In this project we implement our own user-level thread library called
libuthread. We built the project from basics to complex. We went from queues to
threads to semaphores to preemption. We also built our own tests to test each
phase of our project. The tests we built aim to test how resistive our
implementation is.

## Queue Implementation
With the requirement for a FIFO(first in first out) queue and time complexity 
O(1) for manipulating our queue (except deletion and iteration) in mind, we 
decided to use a linked list.
### Queue Structs
We decided to use two structs: node and queue. The node struct holds the data 
and a pointer to the next node. The queue struct has two node pointers pointing 
to the front(head) and back(tail) of the queue, as well as a 'length' variable 
to keep track of queue length.
### Queue Functions
<h5 a><strong><code>queue_t queue_create(void)</code></strong></h5>
This function initializes the queue and returns the created queue.
<h5 a><strong><code>int queue_destroy(queue_t queue)</code></strong></h5>
This function checks if queue is empty and not NULL and then deallocates the
memory for the queue provided to the function. Returns 0 for success and -1 for
failure.
<h5 a><strong><code>int queue_enqueue(queue_t queue, void *data)</code></strong></h5>
This function checks if queue is not NULL and data is not NULL. If true, then  
it adds a node with the 'data' information to the queue. Every time an element 
is added to the queue, it is added to the back of the queue (unless it is the 
first element––then it is set as both the front and back of the queue). Then it 
increments the queue length variable and returns 0 for success (or -1 for 
failure.)
<h5 a><strong><code>int queue_dequeue(queue_t queue, void **data)</code></strong></h5>
This function checks if queue is not NULL and data that is dequeued is not NULL.
If true, it removes the element at the front of the queue (because this is
the oldest item in the queue) and gives this value to 'data'. Then, it 
decrements the queue 'length' variable and returns 0 for success or -1 for 
failure.
<h5 a><strong><code>int queue_delete(queue_t queue, void *data)</code></strong></h5>
This function checks if queue is not NULL, data is not NULL, and the queue is 
not empty. If true, it deletes the oldest node containing the 'data' parameter 
from the queue. It returns 0 for success or -1 for failure.
<h5 a><strong><code>int queue_iterate(queue_t queue, queue_func_t func)</code></strong></h5>
This function checks if queue is not NULL and func parameter is not NULL. If 
true, then it iterates through the entire queue and applies the function func 
to each element. It return 0 for success or -1 for failure.
<h5 a><strong><code>int queue_length(queue_t queue)</code></strong></h5>
This function checks if queue is not NULL. It returns queue length for success 
or -1 for failure.

### Queue Testing
To test our implementation, we use several test cases (see queue_tester.c file)
to catch corner cases (such as enqueuing/dequeueing from a queue that hasn't
been created yet) and ensure correct operation (making sure that deleting a
particular value works correctly).

## UThread Implementation
We use a struct to hold information about each thread's stack and its context.
We used the ucontext library functions to set up, execute and exit each thread.
We declared a global thread struct to keep track of the current thread in 
execution, and also have a global queue called ready_queue to implement
schedule threads based on when they are created.
### UThread Struct
To keep track of the ready threads, we use a queue of TCBs (ready_queue). Each 
TCB is implemented as a uthread_tcb struct, which contains pointers to the 
stack and context.
### UThread Functions
<h5 a><strong><code>void uthread_yield(void)</code></strong></h5>
This function allows the current thread to yield to the next ready thread in the
ready queue. It dequeues the next thread from the ready queue, enqueues the
current process, then context switches to the dequeued next thread.
<h5 a><strong><code>void uthread_exit(void)</code></strong></h5>
This function leaves the current thread and never returns to it, which is why it 
is called exit. This function is called when the thread is done executing and is
ready to become a zombie thread and be collected by the parent process.
<h5 a><strong><code>int uthread_create(uthread_func_t func, void *arg)</code></strong></h5>
This function creates a TCB (uthread_tcb struct) for a new thread. It does this
by allocating memory for the uthread_tcb struct, the stack (via
uthread_ctx_alloc_stack()), and the context, and initializing the context with
the provided function and arg (via uthread_ctx_init()). It then enqueues this to
the ready queue. 
<h5 a><strong><code>int uthread_run(void)</code></strong></h5>
This function acts as the main thread; it is always going to be the "parent"
thread. This function creates the other threads and executes them. This function
keeps running the threads until there are no more threads in the ready_queue. 
Once all the other threads are done executing and exit, this function returns 0 
to exit.

### UThread Testing
We took advantage of the available tester files to test our implementation. We
also used print statements to ensure that the threads were being scheduled in
the expected order, that the final thread scheduled is the idle thread, etc.

## Semaphore Implementation
When threads run concurrently and use the same resources, they can potentially 
change the same resource and cause problems if we don't add proper measures for 
synchronization. To avoid two (or more) threads manipulating the same resource 
simultaneously, we implemented semaphores that control resources. If two 
different threads attempt to access the same resource, one of the threads is put 
in the resource's wait queue, while the other uses the resource. The other 
thread can use the resource when the resource becomes available again.
### Semaphore Struct
To keep track of each semaphore's resource and its availability, we use a
semaphore struct, which contains the semaphore count (can be 0 or a positive
integer) and a wait queue, to keep track of the threads that are waiting to use
the resource.
### Semaphore Functions
<h5 a><strong><code>sem_t sem_create(size_t count)</code></strong></h5>
This function allocates memory for the semaphore and checks if memory allocation 
was successful. It sets the internal count to the parameter count and allocates
the wait_queue for the semaphore as well. Returns the created semaphore struct 
or -1 upon failure.
<h5 a><strong><code>int sem_destroy(sem_t sem)</code></strong></h5>
This function checks if sem is not NULL and if the wait queue is empty. If true,
it deallocates the sem semaphore. Returns 0 for success or -1 for failure. 
<h5 a><strong><code>int sem_down(sem_t sem)</code></strong></h5>
This function checks if sem is not NULL. If true, it checks if semaphore is
available by checking the count. If it is available (count > 0), then it 
decrements it to indicate that is being used. Otherwise (count = 0), it adds the 
current thread to the semaphore's wait queue and blocks it (via the 
uthread_block() function, which dequeues the thread from the ready queue). This 
ensures the thread cannot be scheduled until the semaphore becomes available. 
It returns 0 for success or -1 for failure.
<h5 a><strong><code>int sem_up(sem_t sem)</code></strong></h5>
This function checks if sem is not NULL. If true, it increments the count for 
the semaphore. If there are any threads in the wait queue, it dequeues and 
unblocks the first thread in the wait queue (via uthread_unblock(), which 
enqueues this thread to the ready queue); since the resource is once again 
occupied, it decrements the count. It returns 0 for success or -1 for failure.
<h5 a><strong><code>void uthread_unblock(struct uthread_tcb *uthread)</code></strong></h5>
This function switches from the current thread to the next thread in the 
ready queue (via the uthread_ctx_switch() function). This prevents the current 
thread from being scheduled until it enters the ready queue again.
<h5 a><strong><code>void uthread_unblock(struct uthread_tcb *uthread)</code></strong></h5>
This function enqueues the inputted thread (typically the thread freed from a 
resource's wait queue) onto the ready queue, so that it can be scheduled in the 
future.

### Semaphore Testing
We took advantage of the available tester files to test our implementation.

## Preemption Implementation
We use preemption so that the user does not have to manually call the 
uthread_yield() function to switch threads. With preemption, a given thread is 
not permitted to hold CPU resources indefinitely; rather, the thread will be 
'yielded' during its execution and then later return to continue its execution. 
This allows for fair usage of available resources and prevents threads from 
overusing resources.
### Preemption Functions
<h5 a><strong><code>void preempt_start(bool preempt)</code></strong></h5>
This function checks if preempt is true. It then sets up the signal handler. 
It uses sigaction and setitimer to determine what to do when a signal is 
received, as well as set up a signal alarm of type SIGVTALRM that is sent at a
frequency of 100Hz.
<h5 a><strong><code>void preempt_stop(void)</code></strong></h5>
This function checks if preempt is true. It then restores the previous signal
action and restores the previous timer configuration.
<h5 a><strong><code>void preempt_disable(void)</code></strong></h5>
This function blocks the SIGVTALRM signal using sigprocmask. This allows us to
section off parts of code that we need to avoid interrupting.
<h5 a><strong><code>void preempt_enable(void)</code></strong></h5>
This function unblocks the SIGVTALRM signal using sigprocmask. This allows us to
resume interruptions for parts of code that can be interrupted.

### Preemption Testing
We created a test in test_preempt.c file that runs indefinitely, switching
between threads using the preemptive interrupts. It switches from the main
thread to thread1, thread2, thread3 then back to main using the preemptive
interrupts.

## References
As we debugged, we referenced several online sources to help us figure out a
compiler error:

https://itecnote.com/tecnote/linux-struct-sigaction-incomplete-error

https://stackoverflow.com/questions/6491019/struct-sigaction-incomplete-error
