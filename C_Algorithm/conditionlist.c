#include "conditionlist.h"
#include <stdlib.h>



ConditionList make_CList() {
    ConditionList new_CList = malloc(sizeof(condition_list));
    new_CList->size = 0;
    new_CList->list = malloc(sizeof(pair_list));
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
    for (int index = 0; index < c->size; index++) {
        if (c->list[index] && pairListsEqual(c->list[index], p)) {
            return 1;
        }
    }
    return 0;
}
