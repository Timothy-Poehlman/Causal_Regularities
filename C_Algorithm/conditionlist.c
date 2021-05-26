#include <stdlib.h>
#include <stdio.h>
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
    }
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
}
