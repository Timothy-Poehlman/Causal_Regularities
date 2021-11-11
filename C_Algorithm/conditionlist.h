#ifndef COND
#define COND
#include "pairlist.h"
#include <pthread.h>

typedef struct condition_list *ConditionList;
struct condition_list
{
    PairList list;
    PairList last;
    int size;
    pthread_mutex_t lock;
} condition_list;

ConditionList make_CList();
void CList_add(ConditionList c, PairList p);
void CList_free(ConditionList c);
int CList_contains(ConditionList c, PairList p);
void CList_print(ConditionList c);
void t_setInsert(ConditionList cList, PairList pList);
PairList CList_Pop(ConditionList cList);
ConditionList CList_Copy(ConditionList c);
#endif