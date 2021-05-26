#ifndef COND
#define COND
#include "pairlist.h"

typedef struct condition_list *ConditionList;
struct condition_list
{
    PairList list;
    int size;
} condition_list;

ConditionList make_CList();
void CList_add(ConditionList c, PairList p);
void CList_free(ConditionList c);
int CList_contains(ConditionList c, PairList p);
void CList_print(ConditionList c);
#endif