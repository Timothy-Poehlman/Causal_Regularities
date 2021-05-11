#include "pairlist.h"
#include <stdlib.h>

Pair make_pair(int index, int value)
{
    Pair new_pair = malloc(sizeof(pair_desc));
    new_pair->index = index;
    new_pair->value = value;
    return new_pair;
}

int pairs_equal(Pair p1, Pair p2)
{
    return ((p1->index == p2->index) && (p1->value == p2->value));
}

int pairInList(Pair* pairList, Pair pair)
{
    int pairListLen = sizeof(pairList)/sizeof(Pair);

    for(int index=0;index<pairListLen;index++)
    {
        if(pairs_equal(pair,pairList[index]))
        {
            return 1;
        }
    }
    return 0;
}

int pairListsEqual(Pair* pairList1,Pair* pairList2)
{
    int pairListSize = sizeof(pairList1)/sizeof(Pair);

    for(int p1=0;p1<pairListSize;p1++)
    {
        if(pairInList(pairList2,pairList1[p1]))
        {
            return 1;
        }
    }
    return 0;
}

int pairListInList(Pair** conditionList, Pair* pairList)
{
    int condSize = sizeof(conditionList)/sizeof(conditionList[0]);

    for(int index = 0; index<condSize;index++)
    {
        if(pairListsEqual(conditionList[index], pairList))
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