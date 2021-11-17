#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include "conditionlist.h"



ConditionList make_CList() {
    ConditionList new_CList = malloc(sizeof(condition_list));
    new_CList->size = 0;
    new_CList->list = NULL;
    return new_CList;
}

void CList_add(ConditionList c, PairList p) {
    if (c->list == NULL) {
        c->list = p;
    }
    else {
        PairList current = c->list;
        while (current->next != NULL) {
            current = current->next;
        }
        current->next = p;
        p->prev = current;
    }
    c->size+=1;
}

PairList CList_Pop(ConditionList cList){
    PairList output = cList->list;
    if(!output){
        return NULL;
    }
    PairList new_head = output->next;
    cList->list = new_head;
    if(new_head){
        new_head->prev = NULL;
    }
    output->next = NULL;
    cList->size -= 1;
    return output;
}

void CList_free(ConditionList c) {

}

int CList_contains(ConditionList c, PairList p) {
    PairList tmpList = c->list;
    while(tmpList)
    {
        if(pairListsEqual(tmpList,p))
        {
            return 1;
        }
        tmpList = tmpList->next;
    }
    return 0;
}

void CList_print(ConditionList c) {
    PairList current = c-> list;
    while (current) {
        printPairList(current);
        printf("\n");
        current = current->next;
    }
    printf("\n");
}

ConditionList CList_Copy(ConditionList c){
    ConditionList copy = make_CList();
    PairList current = c->list;
    while(current){
        CList_add(copy,copy_pairList(current));
        current = current->next;
    }
    return copy;
}

int conditionListsEqual(ConditionList cList1, ConditionList cList2) {
    PairList current = cList1->list;
    while (current) {
        if (!CList_contains(cList2, current)) {
            return 0;
        }
        current = current->next;
    }
    current = cList2->list;
    while (current) {
        if (!CList_contains(cList1, current)) {
            return 0;
        }
        current = current->next;
    }
    return 1;
}

void t_setInsert(ConditionList cList, PairList pList) {
    
    pthread_mutex_lock(&(cList->lock));
    if(!CList_contains(cList,pList)){
        CList_add(cList,pList);
    }
    pthread_mutex_unlock(&(cList->lock));
}
