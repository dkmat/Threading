#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

#include <queue.h>

#define TEST_ASSERT(assert)				\
do {									\
	printf("ASSERT: " #assert " ... ");	\
	if (assert) {						\
		printf("PASS\n");				\
	} else	{							\
		printf("FAIL\n");				\
		exit(1);						\
	}									\
} while(0)

/* Test Create */
void test_create(void)
{
	fprintf(stderr, "\n*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Test Enqueue/Dequeue simple: enqueues then dequeues; enqueues then dequeues */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "\n*** TEST queue_simple ***\n");

	q = queue_create();
	queue_enqueue(q, &data);
	int len = queue_length(q);
	fprintf(stderr,"queue length: %d\n", len);
	queue_dequeue(q, (void**)&ptr);
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n", len);
	TEST_ASSERT(ptr == &data);
	queue_enqueue(q,&data);
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n", len);
	queue_dequeue(q, (void**)&ptr);
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n",len);
	TEST_ASSERT(ptr == &data);
}
/* Test Queue Queue: Enqueues twice, dequeues once */
void test_queue_queue(void){
	int data1 = 100, data2 = 4, *ptr;
	queue_t q;
	fprintf(stderr, "\n*** TEST queue_queue ***\n");

	q = queue_create();
	queue_enqueue(q, &data1);
	int len = queue_length(q);
	fprintf(stderr,"queue length: %d\n", len);
	queue_enqueue(q, &data2);
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n", len);

	queue_dequeue(q, (void**)&ptr);
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n", len);
	TEST_ASSERT(ptr == &data1);

}

/* Test Delete: creates a queue and initializes it with values; deletes '5' */
void test_delete(void){
	int *ptr;
	int arr[] = {4,5,6,5,4};
	fprintf(stderr, "\n*** TEST delete ***\n");

	queue_t q = queue_create();
	for (int i = 0; i < 5; i++) {
		queue_enqueue(q, &arr[i]);
	}

	queue_delete(q, &arr[1]); // looks for and deletes '5'
	int len = queue_length(q);
	fprintf(stderr,"queue length: %d\n",len);

	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[2]);

	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[4]);
}

/* Test Delete Delete: creates a queue and initializes it with values; deletes two different values */
void test_delete_delete(void){
	int *ptr;
	int arr[] = {1, 2, 3, 4, 5};
	fprintf(stderr, "\n*** TEST delete delete ***\n");
	
	queue_t q = queue_create();
	for (int i = 0; i < 5; i++) {
		queue_enqueue(q, &arr[i]);
	}

	queue_delete(q, &arr[1]); // looks for and deletes '2'
	int len = queue_length(q);
	fprintf(stderr,"queue length: %d\n",len);

	queue_delete(q, &arr[2]); // looks for and deletes '3'
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n",len);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[0]);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[3]);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[4]);
}

/* Test Delete Before: creates a queue and initializes it with values; deletes two different values, where the second value is earlier in the queue than the first value */
void test_delete_before(void){
	int *ptr;
	int arr[] = {1, 2, 3, 4, 5};
	fprintf(stderr, "\n*** TEST delete before ***\n");
	
	queue_t q = queue_create();
	for (int i = 0; i < 5; i++) {
		queue_enqueue(q, &arr[i]);
	}

	queue_delete(q, &arr[3]); // looks for and deletes '4'
	int len = queue_length(q);
	fprintf(stderr,"queue length: %d\n",len);

	queue_delete(q, &arr[1]); // looks for and deletes '2'
	len = queue_length(q);
	fprintf(stderr,"queue length: %d\n",len);

	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[0]);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[2]);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[4]);
}

/* Test Delete No Value: creates a queue and initializes it with values; tries to delete a value that is not in the queue */
void test_delete_noval(void){
	int arr[] = {1, 2, 3, 4, 5};
	fprintf(stderr, "\n*** TEST delete no value ***\n");
	
	queue_t q = queue_create();
	for (int i = 0; i < 5; i++) {
		queue_enqueue(q, &arr[i]);
	}

	int num = 10;
	queue_delete(q, &num); // looks for and tries to delete '10'
	int len = queue_length(q);
	TEST_ASSERT(len==5); // if length is still 5, 
}

/* Test Destroy: makes sure destroy() is only successful when queue is empty */
void test_destroy(void){
	fprintf(stderr, "\n*** TEST destroy ***\n");
	queue_t q = queue_create();
	int data = 10, *ptr,retval;
	queue_enqueue(q,&data);
	retval = queue_destroy(q); // queue not empty yet
	TEST_ASSERT(retval == -1);
	queue_dequeue(q,(void**)&ptr);
	retval = queue_destroy(q); // queue empty, should successfully destroy
	TEST_ASSERT(retval == 0);
}

static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

/* Test iterator: initialize the queue with values, then increment every value in the queue and perform a deletion */
void test_iterator(void)
{
	fprintf(stderr, "\n*** TEST iterator ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    /* Increment every item of the queue, delete item '42' */
    queue_iterate(q, iterator_inc);
    TEST_ASSERT(data[0] == 2);
    TEST_ASSERT(queue_length(q) == 9);
}

/* Test Enqueue Null: try to enqueue null data */
void test_enqueue_nullval(void)
{
	fprintf(stderr, "\n*** TEST enqueue null val ***\n");
	queue_t q = queue_create();
	int *data = NULL;
	int retval;
	retval = queue_enqueue(q, data);
	TEST_ASSERT(retval == -1);
}

/* Test Enqueue Null Queue: try to enqueue to a null queue */
void test_enqueue_nullq(void)
{
	fprintf(stderr, "\n*** TEST enqueue null queue ***\n");
	queue_t q = NULL;
	int data = 3;
	int retval;
	retval = queue_enqueue(q, &data);
	TEST_ASSERT(retval == -1);
}

/* Test Dequeue Empty Queue: try to dequeue an empty queue */
void test_dequeue_empty(void)
{
	fprintf(stderr, "\n*** TEST dequeue empty ***\n");
	queue_t q = queue_create();
	void **data = NULL;
	int retval;
	retval = queue_dequeue(q, data);
	TEST_ASSERT(retval == -1);
}

/* Test Delete Null: try to delete null value */
void test_delete_nullval(void)
{
	fprintf(stderr, "\n*** TEST delete null val ***\n");
	queue_t q = queue_create();
	int *data = NULL;
	int retval;
	retval = queue_delete(q, data);
	TEST_ASSERT(retval == -1);
}

/* Test Delete Empty Q: try to delete from an empty queue */
void test_delete_empty(void)
{
	fprintf(stderr, "\n*** TEST delete empty ***\n");
	queue_t q = queue_create();
	int three = 3;
	int *data = &three;
	int retval;
	retval = queue_delete(q, data);
	TEST_ASSERT(retval == -1);
}

/* Test Destroy Nexist: try to destroy a nonexistent queue */
void test_destroy_nexist(void)
{
	fprintf(stderr, "\n*** TEST destroy nonexistent queue ***\n");
	queue_t q = NULL;
	int retval;
	retval = queue_destroy(q); // queue not created, should return unsuccessful
	TEST_ASSERT(retval == -1);
}

/* Test Iterator Null Func: try to iterate with a null func name */
void test_iterator_nullfunc(void)
{
	fprintf(stderr, "\n*** TEST iterator null func ***\n");
    queue_t q;
    int data[] = {1, 2, 3, 4, 5, 42, 6, 7, 8, 9};
    size_t i;

    /* Initialize the queue and enqueue items */
    q = queue_create();
    for (i = 0; i < sizeof(data) / sizeof(data[0]); i++)
        queue_enqueue(q, &data[i]);

    int retval = queue_iterate(q, NULL); // iterate should return unsuccessful
    TEST_ASSERT(retval == -1);
}

int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_queue();
	test_delete();
	test_delete_delete();
	test_delete_before();
	test_delete_noval();
	test_destroy();
	test_iterator();
	test_enqueue_nullval();
	test_enqueue_nullq();
	test_dequeue_empty();
	test_delete_empty();
	test_destroy_nexist();
	test_iterator_nullfunc();

	return 0;
}
