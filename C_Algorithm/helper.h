#ifndef HELPER
#define HELPER
#include "conditionlist.h"
#include "queue.h"


ConditionList prepare_table(FILE* input_table);
int rowDuplicity(ConditionList table,int factor);
int intArrayIn(int val, int* array);
int check_sufficient(PairList condition,ConditionList table,int effect);
void permutations(PairList input, int l, int r, Queue* outputQueue);
void swap(Pair* a, Pair* b);
void setFlags(char* flags);
char* subString(char* str, int len);
FILE* setStream(char* arg);

#endif