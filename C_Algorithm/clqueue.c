#include <stdlib.h>
#include <pthread.h>
#include "clqueue.h"

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct CLQueue* createCLQueue(unsigned capacity)
{
    struct CLQueue* queue = (struct CLQueue*)malloc(sizeof(struct CLQueue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;

    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (ConditionList*)malloc(queue->capacity * sizeof(ConditionList));
    queue->addCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    queue->removeCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    return queue;
}

// Queue is full when size becomes
// equal to the capacity
int clIsFull(struct CLQueue* queue)
{
    return (queue->size == queue->capacity);
}

// Queue is empty when size is 0
int clIsEmpty(struct CLQueue* queue)
{
    return (queue->size == 0);
}

// Function to add an item to the queue.
// It changes rear and size
void clEnqueue(struct CLQueue* queue, ConditionList item)
{
    pthread_mutex_lock(&(queue->lock));

    while (1) {
        if (clIsFull(queue))
            pthread_cond_wait(&(queue->addCond), &(queue->lock));
        else {
            printf("adding to queue\n");
            queue->rear = (queue->rear + 1) % queue->capacity;
            queue->array[queue->rear] = item;
            queue->size = queue->size + 1;

            pthread_cond_signal(&(queue->removeCond));
            pthread_mutex_unlock(&(queue->lock));
            break;
        }
    }
}

// Function to remove an item from queue.
// It changes front and size
ConditionList clDequeue(struct CLQueue* queue, int* flag)
{
    pthread_mutex_lock(&(queue->lock));

    while (1) {
        if (clIsEmpty(queue)) {
            if (*flag) {
                printf("exiting\n");
                pthread_mutex_unlock(&(queue->lock));
                pthread_exit(0);
            }
            else
                pthread_cond_wait(&(queue->removeCond), &(queue->lock));
        }
        else {
            printf("removing from queue\n");
            ConditionList item = queue->array[queue->front];
            queue->front = (queue->front + 1) % queue->capacity;
            queue->size = queue->size - 1;

            pthread_cond_signal(&(queue->addCond));
            pthread_mutex_unlock(&(queue->lock));
            return item;
        }
    }
}

// Function to get front of queue
ConditionList clFront(struct CLQueue* queue)
{
    if (clIsEmpty(queue))
        return NULL;
    return queue->array[queue->front];
}

// Function to get rear of queue
ConditionList clRear(struct CLQueue* queue)
{
    if (clIsEmpty(queue))
        return NULL;
    return queue->array[queue->rear];
}