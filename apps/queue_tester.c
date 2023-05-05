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

/* Create */
void test_create(void)
{
	fprintf(stderr, "*** TEST create ***\n");

	TEST_ASSERT(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
	int data = 3, *ptr;
	queue_t q;

	fprintf(stderr, "*** TEST queue_simple ***\n");

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
/* Enqueues twice, dequeues once */
void test_queue_queue(void){
	int data1 = 100, data2 = 4, *ptr;
	queue_t q;
	fprintf(stderr, "*** TEST queue_queue ***\n");

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

/* creates a queue with values, deletes a value */
void test_delete(void){
	int *ptr;
	int arr[] = {4,5,6,5,4};
	fprintf(stderr, "*** TEST delete***\n");
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

void test_destroy(void){
	fprintf(stderr, "*** TEST destroy***\n");
	queue_t q = queue_create();
	int data = 10, *ptr,retval;
	queue_enqueue(q,&data);
	retval = queue_destroy(q); // queue not empty yet
	TEST_ASSERT(retval == -1);
	queue_dequeue(q,(void**)&ptr);
	retval = queue_destroy(q); // queue empty, should successfully destroy
	TEST_ASSERT(retval == 0);
	retval = queue_enqueue(q,&data);
	TEST_ASSERT(retval == -1);
}
static void iterator_inc(queue_t q, void *data)
{
    int *a = (int*)data;

    if (*a == 42)
        queue_delete(q, data);
    else
        *a += 1;
}

void test_iterator(void)
{
	fprintf(stderr, "*** TEST iterator***\n");
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

int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_queue();
	test_delete();
	test_destroy();
	test_iterator();
	return 0;
}
