#include <pthread.h>
#include "thread.h"
#include "conditionlist.h"
#include "helper.h"

/* Main thread for Sufficiency */
void* sufficientThread(void* data){
    threadInfo* info = (threadInfo*)data;
    Queue* permQ = info->q;
    ConditionList condList = info->c;
    ConditionList table = info->t;
    int effect = info->e;
    int flag = (int)*(info->f);

    Pair removedPair = NULL;
    PairList permutation = NULL;
    while(1){
        //grab from quuee
        permutation = dequeue(permQ, flag);
        //perform on perm
        for(int i=0;i<permutation->location;i++){
            //remove
            removedPair = pairList_pop(permutation);
            //is still sufficient?
            if(!check_sufficient(permutation,table,effect)){
                pairList_append(permutation,removedPair);
            }
        }
        //thread_insert, loops through to make sure perm is not already in
        t_setInsert(condList,permutation);
    }
}

threadInfo *infoCreate(Queue* q, ConditionList c, ConditionList t, int e, int* f){
    threadInfo* new_threadInfo = malloc(sizeof(threadInfo));
    new_threadInfo->q = q;
    new_threadInfo->c = c;
    new_threadInfo->t = t;
    new_threadInfo->e = e;
    new_threadInfo->f = f;
    return new_threadInfo;
}