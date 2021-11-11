#include <stdlib.h>
#include "solutionlist.h"

SolutionList create_solutionList() {
	SolutionList new_sList = malloc(sizeof(solution_list));
	new_sList->front = NULL;
	new_sList->end = NULL;
	return new_sList;
}

SolutionNode create_solution_node(ConditionList data) {
	SolutionNode node = malloc(sizeof(struct solution_node));
	node->solution = data;
	node->next = NULL;
	return node;
}

void add(ConditionList cList, SolutionList sList) {
	SolutionNode new_node = create_solution_node(cList);
	pthread_mutex_lock(&(sList->lock));
	if (sList->end) {
		sList->end->next = new_node;
		sList->end = new_node;
	}
	else {
		sList->front = new_node;
		sList->end = new_node;
	}
	pthread_mutex_unlock(&(sList->lock));
}