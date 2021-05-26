#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "conditionlist.h"
#include "helper.h"

int* step0(ConditionList main_table, int* NegFactorSet, int* numPotEffects);
ConditionList step2(ConditionList table, int effect);

int main(int argc, char* argv[])
{
    int NegFactorSet[0];
    int* potential_effects;
    int numPotEffects = 0;
    ConditionList conditionList;

    //read csv
    FILE* stream = fopen("TESTINPUT.csv", "r");


    ConditionList table = prepare_table(stream);

    PairList tmpTable = table->list;
    while(tmpTable)
    {
        for(int e=0; e<tmpTable->location;e++)
        {
            printf("%d",tmpTable->list[e]->value);
        }
        printf("\n");
        tmpTable = tmpTable->next;
    }

    potential_effects = step0(table,NegFactorSet,&numPotEffects);

    //--DEBUG
    printf("Potential Effects: [");
    for(int i=0;i<sizeof(potential_effects)/sizeof(int);i++)
    {
        if(i!=0)
        {printf(",");}
        printf("%d",potential_effects[i]);
    }
    printf("]\n");
    //--

    //steps 2->5
    for(int effect = 0; effect<numPotEffects; effect++)
    {
       printf("_________________Effect Index:%d____________________\n",effect);
       
       printf("ConditionList_________________");
       conditionList = step2(table,potential_effects[effect]);
    //    for(int pairList=0;pairList<maxNumConditions;pairList++)
    //    {
    //        printf(pairList);
    //    }
    //    maxNumConditions = 0;
       printf("end ConditionList_______________________\n");
       free(conditionList);
    }
    free(potential_effects);

}

/* STEP 0
 * FactorTable is a hashtable/dictionary of all factors, factorA -> index | key -> value
 * For everything in W, we will have to test this
 * Over every factor, we are collecting the set of factors that are possible effects, this set is W
 * B: set of all factors, table: C
 */
int* step0(ConditionList table, int* NegFactorSet, int* numPotEffects)
{
    int cols = table->list->location;
    int* W = malloc(sizeof(int)*cols);
    int W_index = 0;
    for (int i=0;i<cols;i++)
    {
        printf("checking index %d\n", i);
        if(!(rowDuplicity(table, i) || intArrayIn(i, NegFactorSet)))
        {
            W[W_index] = i;
            W_index++;
            *numPotEffects++;
        }
    }
    //realloc(W,maxPotEffects*sizeof(int));
    return W;
}


/* STEP 2
    maint_table: the original table of values
    effect: the factor being examined

    conditionList: a list of all sufficient conditions
*/
ConditionList step2(ConditionList table, int effect)
{
    ConditionList conditionList = make_CList();
    PairList tmpPList = table->list;
    PairList condition;
    while(tmpPList)
    {
        condition = make_pairList();
        for(int index=0; index<tmpPList->location; index++)
        {
            if(index != effect)
            {
                pairList_append(condition,make_pair(index,tmpPList->list[index]->value));
            }
        }
        if(check_sufficient(condition, table, effect))
        {
            CList_add(conditionList,condition);
        }
        tmpPList = tmpPList->next;
    }
    return conditionList;
}