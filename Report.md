# User-Level Thread Library
## Kimberly Tong, Daksh Mathur
## Summary
In this project we implement our own user-level thread library called
libuthread. We built the project from basics to complex. We went from queues to
threads to semaphores to preemption. We also built our own tests to test each
phase of our project. The tests we built aim to test how resistive our
implementation is.
## Queue Implementation
Understanding the requirement for us to have time complexity O(1) for
manipulating our queues except delete and iterate we decided to use a linked
list.
### Queue Structs
We decided to use two structs node and queue. The node struct holds the data and
a pointer to the next node. The queue struct has two node pointers pointing to
the front(head) and back(tail) of the queue and a variable to keep track of
queue length.
### Queue Functions
<h5 a><strong><code>queue_t queue_create(void)</code></strong></h5>
This function initializes the queue. We return the created queue.
<h5 a><strong><code>int queue_destroy(queue_t queue)</code></strong></h5>
This function makes sure that the queue is empty and not NULL. If this is true then it deallocates the memory for the queue. It returns 0 for success and -1 for faiure.