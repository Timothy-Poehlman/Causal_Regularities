#include "pairlist.h"
#include <stdlib.h>

Pair make_pair(int index, int value)
{
    Pair new_pair = malloc(sizeof(pair_desc));
    new_pair->index = index;
    new_pair->value = value;
    return new_pair;
}

Pair make_pair_list()
{
    PairList pairList = malloc(sizeof(pair_list));
    pairList->list = malloc(0);
    pairList->size = 0;
    return pairList;
}

Pair make_pair_list_of_list()
{
    PairListofList pairListList = malloc(sizeof(pair_list_list));
    pairListList->list = malloc(0);
    pairListList->size = 0;
    return pairListList;
}

int pairs_equal(Pair p1, Pair p2)
{
    return ((p1->index == p2->index) && (p1->value == p2->value));
}

int pairInList(PairList pairList, Pair pair)
{
    for(int index=0;index<pairList->size;index++)
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
    for(int p1=0;p1<pairList1->size;p1++)
    {
        if(!pairList1->list[p1] || !pairInList(pairList2,pairList1->list[p1]))
        {
            return 0;
        }
    }
    return 1;
}

int pairListInList(PairListofList conditionList, PairList pairList)
{
    for(int index = 0; index<conditionList->size;index++)
    {
        if(conditionList->list[index] && pairListsEqual(conditionList->list[index], pairList))
        {
            return 1;
        }
    }
    return 0;
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