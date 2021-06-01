/* Taken from https://www.geeksforgeeks.org/queue-set-1introduction-and-array-implementation/ */

// C program for array implementation of queue
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "pairlist.h"
 
// A structure to represent a queue
typedef struct Queue {
    pthread_mutex_t lock;
    pthread_cond_t addCond;
    pthread_cond_t removeCond;
    int front, rear, size;
    unsigned capacity;
    PairList* array;
}Queue;

// Function to get rear of queue
PairList rear(struct Queue* queue);

// Function to get front of queue
PairList front(struct Queue* queue);

// Function to remove an item from queue.
// It changes front and size
PairList dequeue(struct Queue* queue);

// Function to add an item to the queue.
// It changes rear and size
void enqueue(struct Queue* queue, PairList item);

// Queue is empty when size is 0
int isEmpty(struct Queue* queue);
 
// Queue is full when size becomes
// equal to the capacity
int isFull(struct Queue* queue);

// function to create a queue
// of given capacity.
// It initializes size of queue as 0
struct Queue* createQueue(unsigned capacity);