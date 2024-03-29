#ifndef SOLUTIONLIST
#define SOLUTIONLIST

#include "conditionlist.h"

typedef struct solution_list* SolutionList;
typedef struct solution_node* SolutionNode;

struct solution_node
{
    ConditionList solution;
    SolutionNode next;
};

struct solution_list
{
    SolutionNode front;
    SolutionNode end;
    pthread_mutex_t lock;
} solution_list;

SolutionList create_solutionList();
SolutionNode create_solution_node(ConditionList data);
void sl_t_setInsert(ConditionList cList, SolutionList sList);
void print_sl(SolutionList sList);
int sl_contains(SolutionList sList, ConditionList cList);
void sl_free(SolutionList sList);

#endif