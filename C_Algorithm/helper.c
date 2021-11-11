#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

ConditionList prepare_table(FILE* input_table)
{
    ConditionList table = make_CList();
    int c = 0;
    char line[2];
    PairList pList = make_pairList();

    while(fgets(line,2,input_table))
    {
        if(line[0] == ',' || line[0] == '\r'){}
        else if(line[0]=='\n')
        {
            c = 0;
            CList_add(table,pList);
            pList = make_pairList();
        }
        else
        {
            pairList_append(pList, make_pair(c,atoi(&line[0])));
            c++;
        }
    }
    return table;
}

/*
    Check if the same row but negative factor already exists, if not, insert into set, else return 1
    int[i][j] table, i= coincidence, j = factor index
*/
int rowDuplicity(ConditionList table,int factor)
{
    ConditionList rowSet = make_CList();
    //loop through every row in table

    PairList current = table->list;
    PairList tmpList = NULL;
    
    while (current != NULL) {
        tmpList = copy_pairList(current);
        tmpList->list[factor]->value = 1 - tmpList->list[factor]->value;
        if (CList_contains(rowSet, tmpList)) {
            free(tmpList);
            return 1;
        }
        tmpList->list[factor]->value = 1 - tmpList->list[factor]->value;
        CList_add(rowSet, tmpList);
        current = current->next;
    }
    return 0;
}

//Checks if a given int is in an array
int intArrayIn(int val, int* array)
{
    for(int i=0;i<sizeof(array);i++)
    {
        if(array[i] == val)
        {
            return 1;
        }
    }
    return 0;
}

/*
 * Checks if a given list of pairs (a coincidence) is sufficient
 */
int check_sufficient(PairList condition,ConditionList table,int effect)
{
    int foundOne = 0;
    int isMatch = 1;
    PairList tmpList = table->list;

    Pair pairLoc;
    
    while(tmpList)
    {
        isMatch = 1;
        for(int index=0;index<condition->location;index++)
        {
            pairLoc = condition->list[index];
            //given pair does NOT match same index in coincidence
            if(pairLoc->value != tmpList->list[pairLoc->index]->value)
            {
                isMatch = 0;
                break;
            }
        }
        if(isMatch)
        {
            foundOne = 1;
            if(tmpList->list[effect]->value == 0)
            {
                return 0;
            }
        }
        tmpList = tmpList->next;
    }
    return foundOne;
}

/* Description: recursively checks each permutation of a given PairList
 * Parameters:
 *              input: a PairList that the permutations will be checked from
 *              l: left bound
 *              r: right bound
 *              outputQueue: The queue that the solutions should be put into
 */
void plPermutations(PairList input, int l, int r, Queue* outputQueue) {
    if (l==r) {
        //output
        PairList copy = copy_pairList(input);
        enqueue(outputQueue,copy);
    }
    else {
        for (int i = l; i <= r;i++) {
            plSwap(input->list + l, input->list + i);
            plPermutations(input, l + 1, r, outputQueue);
            plSwap(input->list + l, input->list + i);
        }
    }
}

void plSwap(Pair* a, Pair* b) {
    Pair temp;
    temp = *a;
    *a = *b;
    *b = temp;
}

void clPermutations(ConditionList input, PairList l, CLQueue* outputQueue) {
    PairList current = l;
    if (!current) {
        //output
        ConditionList copy = CList_Copy(input);
        printf("enqueuinging\n");
        clEnqueue(outputQueue, copy);
    }
    else {
        while (current) {
            clSwap(l, current);
            clPermutations(input, l->next, outputQueue);
            clSwap(l, current);
            current = current->next;
        }
    }
}

void clSwap(PairList pl1, PairList pl2) {
    PairList pl1Prev = pl1->prev;
    PairList pl1Next = pl1->next;
    pl1->prev = pl2->prev;
    pl1->next = pl2->next;
    pl2->prev = pl1Prev;
    pl2->next = pl1Next;
}

void setFlags(char* flags)
{
    int len = strlen(flags);
    for(int i=1;i<len;i++)
    {
        switch(flags[i])
        {
            case 'd':
                //debug = 1; What is this supposed to do? Just found it sitting there -Forest
                break;
        }
    }
}

char* subString(char* str, int len)
{
    char* new = malloc(sizeof(char) * len + 1);
    strncpy(new, str, len);
    new[len] = '\0';
    return new;
}

FILE* setStream(char* arg)
{
    char* sString = subString(arg,strlen(arg)-1);
    FILE* stream = fopen(sString+1, "r");
    return stream;
}

int check_necessary(ConditionList table, ConditionList conditions, int effect)
{
    PairList row = table -> list;
    int necessary;
    int found_match;

    //For every row in table
    while(row)
    {
        //if effect is there
        if(row->list[effect]->value)
        {
            necessary = 0;
            //for condition in conditions
            PairList condition = conditions->list;
            while(condition)
            {
                found_match = 1;
                //for pair in condition
                for(int i=0;i<condition->location;i++)
                {
                    Pair pair = condition->list[i];
                    if(pair->value != row->list[pair->index]->value)
                    {
                        found_match = 0;
                    }
                }
                if(found_match)
                {
                    necessary = 1;
                    break;
                }

                condition = condition->next;
            }
            if(necessary == 0)
            {
                return 0;
            }
        }
        row = row->next;
    }
    return 1;
}