/* Taken from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/ */


#ifndef CLQUEUE
#define CLQUEUE
// C program for array implementation of queue
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "conditionlist.h"

// A structure to represent a queue
typedef struct CLQueue {
    pthread_mutex_t lock;
    pthread_cond_t addCond;
    pthread_cond_t removeCond;
    int front, rear, size;
    unsigned capacity;
    ConditionList* array;
}CLQueue;

// Function to get rear of queue
ConditionList clRear(struct CLQueue* queue);

// Function to get front of queue
ConditionList clFront(struct CLQueue* queue);

// Function to remove an item from queue.
// It changes front and size
ConditionList clDequeue(struct CLQueue* queue, int* flag);

// Function to add an item to the queue.
// It changes rear and size
void clEnqueue(struct CLQueue* queue, ConditionList item);

// Queue is empty when size is 0
int clIsEmpty(struct CLQueue* queue);

// Queue is full when size becomes
// equal to the capacity
int clIsFull(struct CLQueue* queue);

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct CLQueue* createCLQueue(unsigned capacity);

void clQueueFree(struct CLQueue* queue);

#endif