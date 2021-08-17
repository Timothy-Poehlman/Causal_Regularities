#ifndef THREAD
#define THREAD

#include "queue.h"
#include "conditionlist.h"

typedef struct threadInfo {
    Queue* q;
    ConditionList c;
    ConditionList t;
    int e;
    int* f;
}threadInfo;

void* sufficientThread(void* data);
threadInfo* infoCreate(Queue* q, ConditionList c, ConditionList t, int e, int* f);

#endif