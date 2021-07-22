#ifndef THREAD
#define THREAD

typedef struct threadInfo {
    Queue* q;
    ConditionList c;
    ConditionList t;
    int e;
    int* f;
}threadInfo;

#endif