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
	fprintf(stderr,"%d\n", len);
	queue_dequeue(q, (void**)&ptr);
	len = queue_length(q);
	fprintf(stderr,"%p %p \n%d\n",ptr,&data,len);
	TEST_ASSERT(ptr == &data);
	queue_enqueue(q,&data);
	len = queue_length(q);
	fprintf(stderr,"%d\n", len);
	queue_dequeue(q, (void**)&ptr);
	len = queue_length(q);
	fprintf(stderr,"%p %p \n%d\n",ptr,&data,len);
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
	fprintf(stderr,"%d\n", len);
	queue_enqueue(q, &data2);
	len = queue_length(q);
	fprintf(stderr,"%d\n", len);

	queue_dequeue(q, (void**)&ptr);
	len = queue_length(q);
	fprintf(stderr,"%p %p \n%d\n",ptr,&data1,len);
	TEST_ASSERT(ptr == &data1);

}

/* creates a queue with values, deletes a value */
void test_delete(void){
	int *ptr;
	int arr[] = {4,5,6,5,4};
	queue_t q = queue_create();
	for (int i = 0; i < 5; i++) {
		queue_enqueue(q, arr[i]);
	}

	queue_delete(q, arr[1]); // looks for and deletes '5'
	int len = queue_length(q);
	fprintf(stderr,"%d\n",len);

	queue_dequeue(q, (void**)&ptr);
	queue_dequeue(q, (void**)&ptr);
	TEST_ASSERT(ptr == &arr[2]);
	
}
int main(void)
{
	test_create();
	test_queue_simple();
	test_queue_queue();
	test_delete();
	return 0;
}
