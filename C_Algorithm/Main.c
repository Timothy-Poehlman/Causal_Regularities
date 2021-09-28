#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "conditionlist.h"
#include "helper.h"
#include "thread.h"

int *step0(ConditionList main_table, int *NegFactorSet, int *numPotEffects);
ConditionList step2(ConditionList table, int effect);
Queue* step3(ConditionList inputConditions, ConditionList table, int effect);

int debug = 1;
int numThreads = 8;

/* MAIN
 * This is the general structure for analyzing a given table
 */
int main(int argc, char *argv[])
{
    int NegFactorSet[0];
    int *potential_effects;
    int numPotEffects = 0;
    ConditionList conditionList;
    Queue *sufficientSet;

    //read csv - if given a csv in "NAME.csv", it will override this
    FILE *stream = fopen("TESTINPUT.csv", "r");

    //check argumentss
    /*  setStream causing issues
    for (int i = 0; i < argc; i++)
    {
        char* arg = argv[i];
        switch (arg[0])
        {
        case '-':
            setFlags(arg);
            break;
        case '\"':
            stream = setStream(arg);
        }
    }
    */

    ConditionList table = prepare_table(stream);

    CList_print(table);

    potential_effects = step0(table, NegFactorSet, &numPotEffects);

    //--DEBUG
    if (debug)
    {
        printf("Potential Effects: [");
        for (int i = 0; i < sizeof(potential_effects) / sizeof(int); i++)
        {
            if (i != 0)
            {
                printf(",");
            }
            printf("%d", potential_effects[i]);
        }
        printf("]\n");
    }
    //--

    //steps 2->5
    for (int effect = 0; effect < numPotEffects; effect++)
    {
        printf("_________________Effect Index:%d____________________\n", potential_effects[effect]);

        conditionList = step2(table, potential_effects[effect]);

        //Debug
        if (debug)
        {
            printf("ConditionList_________________\n");
            PairList currentList = conditionList->list;
            while (currentList)
            {
                printPairList(currentList);
                printf("\n");
                currentList = currentList->next;
            }
            printf("end ConditionList_______________________\n");
        }
        sufficientSet = step3(conditionList, table, potential_effects[effect]);

        free(conditionList);
    }
    free(potential_effects);
}

/* STEP 0
 * table: Main table of all coincidences
 * NegFactorSet: int array of all negative factors, this should be empty
 *               as they should be removed before passing it in
 */
int *step0(ConditionList table, int *NegFactorSet, int *numPotEffects)
{
    int cols = table->list->location;
    int *W = malloc(sizeof(int) * cols);
    int W_index = 0;
    for (int i = 0; i < cols; i++)
    {
        printf("checking index %d\n", i);
        if (!(rowDuplicity(table, i) || intArrayIn(i, NegFactorSet)))
        {
            W[W_index] = i;
            W_index++;
            (*numPotEffects)++;
        }
    }
    W = realloc(W, (*numPotEffects) * sizeof(int));
    return W;
}

/* STEP 2
    table: Main table of all coincidences
    effect: The effect that is being analyzed
*/
ConditionList step2(ConditionList table, int effect)
{
    ConditionList conditionList = make_CList();
    PairList tmpPList = table->list;
    PairList condition;
    while (tmpPList)
    {
        condition = make_pairList();
        for (int index = 0; index < tmpPList->location; index++)
        {
            if (index != effect)
            {
                pairList_append(condition, make_pair(index, tmpPList->list[index]->value));
            }
        }
        if (check_sufficient(condition, table, effect))
        {
            CList_add(conditionList, condition);
        }
        tmpPList = tmpPList->next;
    }
    return conditionList;
}

/* STEP 3
 * inputConditions: ConditionList produced from [STEP 2]
 * table: Main table of all coincidences
 * effect: The effect that is being analyzed
 */
Queue* step3(ConditionList inputConditions, ConditionList table, int effect)
{
    ConditionList minimally_sufficient_conditions = make_CList();

    Queue *queue = createQueue(numThreads);

    int isDone = 0;

    pthread_t threadIds[numThreads];
    threadInfo *info = infoCreate(queue, minimally_sufficient_conditions, table, effect, &isDone);

    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&threadIds[i], NULL, &sufficientThread, info);
    }

    // Loop through each condition, check all permutations, add them if they pass to [queue]
    PairList current = inputConditions->list;
    while (current)
    {
        permutations(current, 0, current->location - 1, queue);
        current = current->next;
    }
    // Set flag to notify we are done creating permutations
    *(info->f) = 1;
    pthread_cond_broadcast(&(queue->removeCond));
    
    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threadIds[i], NULL);
    }

    //return conditionlist from threadinfo instead of queue

    return queue;
}

/*
maint_table: the original table of values
effect : the factor being examined
conditionList : a list of all sufficient conditions

minimally_sufficient_conditions : a set of all minimally sufficient conditions
'''
def step3(conditionList, main_table, effect) :
    minimally_sufficient_conditions = []

    for condition in conditionList :
        prm_list = list(permutations(range(0, len(condition))))
        for prm in prm_list :
            #create pairlist of
            modif_condition = []
            for index in prm :
                modif_condition.append(condition[index])
            tmp_list = copy.deepcopy(modif_condition)
            for factor in tmp_list :
                #remove
                modif_condition.remove(factor)
                #is sufficient ?
                if not check_sufficient(modif_condition, table, effect) :
                    modif_condition.append(factor)
            if not pairListIn(minimally_sufficient_conditions, modif_condition) :
                minimally_sufficient_conditions.append(modif_condition)

    return minimally_sufficient_conditions
*/