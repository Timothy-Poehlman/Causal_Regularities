#ifndef THREAD
#define THREAD

#include "queue.h"
#include "clqueue.h"
#include "conditionlist.h"
#include "solutionlist.h"

/*
* q: queue of permutations (pairlists)
* c: minimally sufficient conditions (output)
* t: the table
* e: the effect
* f: flag for exiting
*/
typedef struct threadInfo {
    Queue* q;
    ConditionList c;
    ConditionList t;
    int e;
    int* f;
}threadInfo;

typedef struct clThreadInfo {
    CLQueue* q;
    SolutionList s;
    ConditionList t;
    int e;
    int* f;
}clThreadInfo;

void* sufficientThread(void* data);
void* necessaryThread(void* data);

threadInfo* infoCreate(Queue* q, ConditionList c, ConditionList t, int e, int* f);
clThreadInfo* clInfoCreate(CLQueue* q, SolutionList s, ConditionList t, int e, int* f);

#endif