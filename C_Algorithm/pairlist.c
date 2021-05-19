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

int pairInList(Pair* pairList, Pair pair, int rows, int cols)
{
    for(int index=0;index<cols;index++)
    {
        if(pairList[index] && pairs_equal(pair,pairList[index]))
        {
            return 1;
        }
    }
    return 0;
}

int pairListsEqual(Pair* pairList1,Pair* pairList2, int rows, int cols)
{
    for(int p1=0;p1<cols;p1++)
    {
        if(!pairList1[p1] || !pairInList(pairList2,pairList1[p1], rows, cols))
        {
            return 0;
        }
    }
    return 1;
}

int pairListInList(Pair** conditionList, Pair* pairList, int rows, int cols)
{
    for(int index = 0; index<rows;index++)
    {
        if(conditionList[index] && pairListsEqual(conditionList[index], pairList, rows, cols))
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