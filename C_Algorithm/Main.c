#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "conditionlist.h"
#include "solutionlist.h"
#include "helper.h"
#include "thread.h"

int *step0(ConditionList main_table, int *neg_factor_set, int *num_pot_effects);
ConditionList step2(ConditionList table, int effect);
ConditionList step3(ConditionList input_conditions, ConditionList table, int effect);
SolutionList step6(ConditionList table, ConditionList necessary_conditions, int effect);

int debug = 1;
int num_threads = 8;

/* MAIN
 * This is the general structure for analyzing a given table
 */
int main(int argc, char *argv[])
{
    int neg_factor_set[0];
    int *potential_effects;
    int num_pot_effects = 0;
    ConditionList condition_list;
    ConditionList sufficient_set;
    SolutionList solutions;

    //read csv - if given a csv in "NAME.csv", it will override this
    FILE *stream = fopen("zoo_cleaned.csv", "r");

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

    potential_effects = step0(table, neg_factor_set, &num_pot_effects);

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
    for (int effect = 0; effect < num_pot_effects; effect++)
    {
        printf("_________________Effect Index:%d____________________\n", potential_effects[effect]);

        condition_list = step2(table, potential_effects[effect]);

        //Debug
        if (debug)
        {
            printf("Sufficient Conditions:\n");
            PairList current_list = condition_list->list;
            while (current_list)
            {
                printPairList(current_list);
                printf("\n");
                current_list = current_list->next;
            }
            printf("\n");
        }
        
        sufficient_set = step3(condition_list, table, potential_effects[effect]);
        
        printf("Minimally Sufficient Conditions:\n");
        CList_print(sufficient_set);
        printf("\n");

        CList_free(condition_list);

        //step 5
        if(check_necessary(table, sufficient_set, potential_effects[effect]))
        {
            printf("started necessary checking\n");
            //step 6
            solutions = step6(table, sufficient_set, potential_effects[effect]);
            printf("Minimally Sufficient Minimally Necessary Conditions:\n");
            print_sl(solutions);
        }
        //CList_free(sufficient_set);
        //sl_free(solutions);
    }
    //CList_free(table);
    //free(potential_effects);
}

/* STEP 0
 * table: Main table of all coincidences
 * neg_factor_set: int array of all negative factors, this should be empty
 *               as they should be removed before passing it in
 */
int *step0(ConditionList table, int *neg_factor_set, int *num_pot_effects)
{
    int cols = table->list->location;
    int *w = malloc(sizeof(int) * cols);
    int w_index = 0;
    for (int i = 0; i < cols; i++)
    {
        if (!(rowDuplicity(table, i) || intArrayIn(i, neg_factor_set)))
        {
            w[w_index] = i;
            w_index++;
            (*num_pot_effects)++;
        }
    }
    w = realloc(w, (*num_pot_effects) * sizeof(int));
    return w;
}

/* STEP 2
    table: Main table of all coincidences
    effect: The effect that is being analyzed
*/
ConditionList step2(ConditionList table, int effect)
{
    ConditionList condition_list = make_CList();
    PairList tmp_p_list = table->list;
    PairList condition;
    while (tmp_p_list)
    {
        condition = make_pairList();
        for (int index = 0; index < tmp_p_list->location; index++)
        {
            if (index != effect)
            {
                pairList_append(condition, make_pair(index, tmp_p_list->list[index]->value));
            }
        }
        if (check_sufficient(condition, table, effect))
        {
            CList_add(condition_list, condition);
        }
        else {
            pairList_free(condition);
        }
        tmp_p_list = tmp_p_list->next;
    }
    return condition_list;
}

/* STEP 3
 * input_conditions: ConditionList produced from [STEP 2]
 * table: Main table of all coincidences
 * effect: The effect that is being analyzed
 */
ConditionList step3(ConditionList input_conditions, ConditionList table, int effect)
{
    ConditionList minimally_sufficient_conditions = make_CList();

    Queue *queue = createQueue(num_threads);

    int is_done = 0;

    pthread_t thread_ids[num_threads];
    threadInfo *info = infoCreate(queue, minimally_sufficient_conditions, table, effect, &is_done);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&thread_ids[i], NULL, &sufficientThread, info);
    }

    // Loop through each condition, check all permutations, add them if they pass to [queue]
    PairList current = input_conditions->list;
    int num_perms = 0;
    while (current)
    {
        plPermutations(current, 0, current->location - 1, queue);
        current = current->next;
        num_perms++;
        printf("%d/%d, %d remaining\n", num_perms, input_conditions->size, queue->size);
    }
    // Set flag to notify we are done creating permutations
    *(info->f) = 1;
    pthread_cond_broadcast(&(queue->removeCond));
    
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(thread_ids[i], NULL);
    }

    free(info);
    queueFree(queue);

    return minimally_sufficient_conditions;
}


/* Minimally Necessary Minimally Sufficient Conditions
 * 
 */

SolutionList step6(ConditionList table, ConditionList necessary_conditions, int effect)
{
    SolutionList minimally_necessary_conditions = create_solutionList();

    CLQueue *queue = createCLQueue(num_threads);

    int is_done = 0;

    pthread_t thread_ids[num_threads];
    clThreadInfo *info = clInfoCreate(queue, minimally_necessary_conditions, table, effect, &is_done);

    for (int i = 0; i < num_threads; i++)
    {
        pthread_create(&thread_ids[i], NULL, &necessaryThread, info);
    }

    clPermutations(necessary_conditions, necessary_conditions->list, queue);

    // Set flag to notify we are done creating permutations
    *(info->f) = 1;
    pthread_cond_broadcast(&(queue->removeCond));
    
    for (int i = 0; i < num_threads; i++)
    {
        pthread_join(thread_ids[i], NULL);
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