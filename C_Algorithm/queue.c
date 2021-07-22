#include <stdlib.h>
#include <pthread.h>
#include "queue.h"

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity)
{
    struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
    queue->capacity = capacity;
    queue->front = queue->size = 0;
 
    // This is important, see the enqueue
    queue->rear = capacity - 1;
    queue->array = (PairList*)malloc(queue->capacity * sizeof(PairList));
    queue->addCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    queue->removeCond = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
    return queue;
}
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue)
{
    return (queue->size == queue->capacity);
}
 
// Queue is empty when size is 0
int isEmpty(struct Queue* queue)
{
    return (queue->size == 0);
}
 
// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, PairList item)
{
    pthread_mutex_lock(&(queue->lock));

    while (1) {
        if (isFull(queue))
            pthread_cond_wait(&(queue->addCond), &(queue->lock));
        else {
            queue->rear = (queue->rear + 1) % queue->capacity;
            queue->array[queue->rear] = item;
            queue->size = queue->size + 1;

            pthread_cond_broadcast(&(queue->removeCond));
            pthread_mutex_unlock(&(queue->lock));
            break;
        }
    }
}
 
// Function to remove an item from queue.
// It changes front and size
PairList dequeue(struct Queue* queue, int flag)
{
    pthread_mutex_lock(&(queue->lock));

    while (1) {
        if (isEmpty(queue))
            if(flag)
                pthread_exit(0);
            else
                pthread_cond_wait(&(queue->addCond), &(queue->lock));
        else {
            PairList item = queue->array[queue->front];
            queue->front = (queue->front + 1) % queue->capacity;
            queue->size = queue->size - 1;

            pthread_mutex_unlock(&(queue->lock));
            return item;
        }
    }
}
 
// Function to get front of queue
PairList front(struct Queue* queue)
{
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->front];
}
 
// Function to get rear of queue
PairList rear(struct Queue* queue)
{
    if (isEmpty(queue))
        return NULL;
    return queue->array[queue->rear];
}