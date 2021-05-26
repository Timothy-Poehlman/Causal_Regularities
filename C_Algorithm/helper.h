#ifndef HELPER
#define HELPER
#include "conditionlist.h"


ConditionList prepare_table(FILE* input_table);
int rowDuplicity(ConditionList table,int factor);
int intArrayIn(int val, int* array);
int check_sufficient(PairList condition,ConditionList table,int effect);

#endif