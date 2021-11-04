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

Pair copy_pair(Pair p) {
    Pair output = malloc(sizeof(pair_desc));
    output->index = p->index;
    output->value = p->value;
    return output;
}

PairList make_pairList()
{
    PairList pairList = malloc(sizeof(pair_list));
    pairList->list = malloc(sizeof(Pair));
    pairList->size = 1;
    pairList->location = 0;
    pairList->next = NULL;
    pairList->prev = NULL;
    return pairList;
}

PairList copy_pairList(PairList p) {
    PairList output = malloc(sizeof(pair_list));
    Pair* templist = malloc(sizeof(Pair) * p->location);
    for (int i = 0; i < p->location;i++) {
        templist[i] = copy_pair(p->list[i]);
    }
    output->list = templist;
    output->size = p->location;
    output->location = p->location;
    output->next = NULL;
    output->prev = NULL;
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

Pair pairList_pop(PairList p){
    if(p->location == 0){return NULL;}

    Pair output = p->list[0];
    for(int i=1;i<p->location;i++){
        p->list[i-1] = p->list[i];
    }
    p->location--;
    return output;
}