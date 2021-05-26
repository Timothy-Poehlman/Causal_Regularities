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