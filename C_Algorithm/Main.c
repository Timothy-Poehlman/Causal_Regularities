#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "conditionlist.h"
#include "solutionlist.h"
#include "helper.h"
#include "thread.h"

int *step0(ConditionList main_table, int *NegFactorSet, int *numPotEffects);
ConditionList step2(ConditionList table, int effect);
ConditionList step3(ConditionList inputConditions, ConditionList table, int effect);
SolutionList step6(ConditionList table, ConditionList necessary_conditions, int effect);

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
    ConditionList sufficientSet;
    SolutionList solutions;

    //read csv - if given a csv in "NAME.csv", it will override this
    FILE *stream = fopen("zoo_cleanedl.csv", "r");

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
            printf("Sufficient Conditions:\n");
            PairList currentList = conditionList->list;
            while (currentList)
            {
                printPairList(currentList);
                printf("\n");
                currentList = currentList->next;
            }
            printf("\n");
        }
        
        sufficientSet = step3(conditionList, table, potential_effects[effect]);
        
        printf("Minimally Sufficient Conditions:\n");
        CList_print(sufficientSet);
        printf("\n");

        CList_free(conditionList);

        //step 5
        if(check_necessary(table, sufficientSet, potential_effects[effect]))
        {
            //step 6
            solutions = step6(table, sufficientSet, potential_effects[effect]);
            printf("Minimally Sufficient Minimally Necessary Conditions:\n");
            print_sl(solutions);
        }
        //CList_free(sufficientSet);
        //sl_free(solutions);
    }
    //CList_free(table);
    //free(potential_effects);
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
        else {
            pairList_free(condition);
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
ConditionList step3(ConditionList inputConditions, ConditionList table, int effect)
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
        plPermutations(current, 0, current->location - 1, queue);
        current = current->next;
    }
    // Set flag to notify we are done creating permutations
    *(info->f) = 1;
    pthread_cond_broadcast(&(queue->removeCond));
    
    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threadIds[i], NULL);
    }

    free(info);
    queueFree(queue);

    return minimally_sufficient_conditions;
}


/* Minimally Necessary Minimally Sufficient Conditions
 * NEEDS REVISION **
 */

SolutionList step6(ConditionList table, ConditionList necessary_conditions, int effect)
{
    SolutionList minimally_necessary_conditions = create_solutionList();

    CLQueue *queue = createCLQueue(numThreads);

    int isDone = 0;

    pthread_t threadIds[numThreads];
    clThreadInfo *info = clInfoCreate(queue, minimally_necessary_conditions, table, effect, &isDone);

    for (int i = 0; i < numThreads; i++)
    {
        pthread_create(&threadIds[i], NULL, &necessaryThread, info);
    }

    clPermutations(necessary_conditions, necessary_conditions->list, queue);

    // Set flag to notify we are done creating permutations
    *(info->f) = 1;
    pthread_cond_broadcast(&(queue->removeCond));
    
    for (int i = 0; i < numThreads; i++)
    {
        pthread_join(threadIds[i], NULL);
    }

    free(info);
    clQueueFree(queue);

    return minimally_necessary_conditions;
}

/*'''
maint_table: the original table of values
effect: the factor being examined
necessary_condition: the necessary condition (not minimized)

pair structure -> (index, value)

minimally_necessary_conditions: the conditions that are minimally necessary for the given effect
'''
def step6(main_table, necessary_condition, effect):
    minimally_necessary_conditions = []
    prm_list = list(permutations(range(0, len(necessary_condition))))
    for prm in prm_list:
        test_necessary_condition = []
        for index in prm:
            test_necessary_condition.append(necessary_condition[index])
        for i in range(0, len(test_necessary_condition)):
            suff_cond = test_necessary_condition.pop(0)
            if not check_necessary(main_table, test_necessary_condition, effect):
                test_necessary_condition.append(suff_cond)
        if not pairListListIn(minimally_necessary_conditions, test_necessary_condition):
            minimally_necessary_conditions.append(test_necessary_condition)
    return minimally_necessary_conditions
    */