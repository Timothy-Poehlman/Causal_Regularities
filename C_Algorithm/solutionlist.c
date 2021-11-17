#include <stdio.h>
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

int sl_contains(SolutionList sList, ConditionList cList) {
	SolutionNode current = sList->front;
	while (current) {
		if (conditionListsEqual(current->solution, cList)) {
			return 1;
		}
		current = current->next;
	}
	return 0;
}

void sl_t_setInsert(ConditionList cList, SolutionList sList) {
	SolutionNode new_node = create_solution_node(cList);
	pthread_mutex_lock(&(sList->lock));
	if (sl_contains(sList, cList)) {
		pthread_mutex_unlock(&(sList->lock));
		return;
	}
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

void print_sl(SolutionList sList){
	printf("printing sol list, first is size %d\n",sList->front->solution->size);
	if(!sList->front->solution){
		printf("empty\n");
	}
	SolutionNode current = sList->front;
	while(current){
		CList_print(current->solution);
		current = current->next;
	}
	printf("finished printing SL\n");
}
