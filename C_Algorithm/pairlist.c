#include <stdlib.h>
#include <stdio.h>
#include "pairlist.h"

Pair make_pair(int index, int value)
{
    Pair new_pair = malloc(sizeof(pair_desc));
    new_pair->index = index;
    new_pair->value = value;
    return new_pair;
}

PairList make_pairList()
{
    PairList pairList = malloc(sizeof(pair_list));
    pairList->list = malloc(sizeof(Pair));
    pairList->size = 1;
    pairList->location = 0;
    return pairList;
}

PairList copy_pairList(PairList p) {
    PairList output = malloc(sizeof(pair_list));
    Pair* templist = malloc(sizeof(Pair) * p->size);
    for (int i = 0; i < p->size;i++) {
        templist[i] = p->list[i];
    }
    output->list = templist;
    output->size = p->size;
    output->location = p->location;
    output->next = NULL;
    return output;
}

/* appends p2 to the end of p1 */
void pairList_append(PairList p1, Pair p2)
{
    if(p1->location>=p1->size)
    {
        Pair* tmp = malloc((p1->size)*2 * sizeof(Pair));
        for(int i=0;i<p1->size;i++)
        {
            tmp[i] = p1->list[i];
        }
        free(p1->list);
        p1->list = tmp;
        p1->size = p1->size * 2;
    }
    p1->list[p1->location] = p2;
    p1->location = p1->location + 1;
}

int pairs_equal(Pair p1, Pair p2)
{
    return ((p1->index == p2->index) && (p1->value == p2->value));
}

int pairInList(PairList pairList, Pair pair)
{
    for(int index=0;index<pairList->location;index++)
    {
        if(pairList->list[index] && pairs_equal(pair,pairList->list[index]))
        {
            return 1;
        }
    }
    return 0;
}

int pairListsEqual(PairList pairList1,PairList pairList2)
{
    for(int p1=0;p1<pairList1->location;p1++)
    {
        if(!pairList1->list[p1] || !pairInList(pairList2,pairList1->list[p1]))
        {
            return 0;
        }
    }
    return 1;
}

void printPairList(PairList pl)
{
    for(int i=0;i<pl->location;i++)
    {
        printf("(i:%d,v:%d)",pl->list[i]->index,pl->list[i]->value);
    }
}

// def pairListListsEqual(Pair** pairListList1, Pair** pairListList2):
//     if len(pairListList1) == len(pairListList2):
//         for pairList in pairListList1:
//             if not pairListIn(pairListList2, pairList):
//                 return False
//         return True
//     return False

// def pairListListIn(conditionList, pairListList):
//     for elem in conditionList:
//         if pairListListsEqual(elem, pairListList):
//             return True
//     return False