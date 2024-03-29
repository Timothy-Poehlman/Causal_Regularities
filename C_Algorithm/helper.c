#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "helper.h"

static int perm_count = 0;


//NEEDS REWORK CAUSES SMALL MEMORY LEAK BY CREATING AN EXTRA PAIRLIST
ConditionList prepare_table(FILE* input_table)
{
    ConditionList table = make_CList();
    int c = 0;
    char line[2];
    PairList pList = make_pairList();

    while (fgets(line, 2, input_table))
    {
        if (line[0] == ',' || line[0] == '\r') {}
        else if (line[0] == '\n')
        {
            c = 0;
            CList_add(table, pList);
            pList = make_pairList();
        }
        else
        {
            pairList_append(pList, make_pair(c, atoi(&line[0])));
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
            pairList_free(tmpList);
            CList_free(rowSet);
            return 1;
        }
        tmpList->list[factor]->value = 1 - tmpList->list[factor]->value;
        CList_add(rowSet, tmpList);
        current = current->next;
    }
    CList_free(rowSet);
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
    perm_count = 0;
    plPermutationsHelper(input, l, r, outputQueue);
}


void plPermutationsHelper(PairList input, int l, int r, Queue* outputQueue) {
    if (l == r) {
        //output
        perm_count++;
        if(perm_count % 100000 == 0)
            printf("%d\n", perm_count);
        PairList copy = copy_pairList(input);
        enqueue(outputQueue, copy);
    }
    else {
        for (int i = l; i <= r; i++) {
            plSwap(input->list + l, input->list + i);
            plPermutationsHelper(input, l + 1, r, outputQueue);
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
    perm_count = 0;
    clPermutationsHelper(input, l, outputQueue);
}

void clPermutationsHelper(ConditionList input, PairList l, CLQueue* outputQueue) {
    PairList current = l;
    if (!current) {
        //output
        perm_count++;
        if (perm_count % 10000000 == 0)
            printf("%d\n", perm_count);
        ConditionList copy = CList_Copy(input);
        clEnqueue(outputQueue, copy);
    }
    else {
        while (current) {
            clSwap(l, current);
            clPermutationsHelper(input, l->next, outputQueue);
            clSwap(l, current);
            current = current->next;
        }
    }
}

void clSwap(PairList pl1, PairList pl2) {
    Pair* list1 = pl1->list;
    int size1 = pl1->size;
    int loc1 = pl1->location;

    pl1->list = pl2->list;
    pl1->size = pl2->size;
    pl1->location = pl2->location;
    pl2->list = list1;
    pl2->size = size1;
    pl2->location = loc1;
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

/*
 * necessary returns 1
 * not returns 0
 */
int check_necessary(ConditionList table, ConditionList conditions, int effect)
{

    PairList row = table -> list;
    int necessary;
    int found_match;

    //For every row in table
    while(row){
        //if effect is there
        if(row->list[effect]->value){
            necessary = 0;
            //for condition in conditions
            PairList condition = conditions->list;
            while(condition){
                found_match = 1;
                //for pair in condition
                for(int i=0;i<condition->location;i++){
                    Pair pair = condition->list[i];
                    if(pair->value != row->list[pair->index]->value){
                        found_match = 0;
                    }
                }
                if(found_match){
                    necessary = 1;
                    break;
                }

                condition = condition->next;
            }
            if(necessary == 0){
                return 0;
            }
        }
        row = row->next;
    }
    return 1;
}