#ifndef HELPER
#define HELPER
#include "conditionlist.h"
#include "queue.h"
#include "clqueue.h"


ConditionList prepare_table(FILE* input_table);
int rowDuplicity(ConditionList table, int factor);
int intArrayIn(int val, int* array);
int check_sufficient(PairList condition, ConditionList table, int effect);
void plPermutations(PairList input, int l, int r, Queue* outputQueue);
void plSwap(Pair* a, Pair* b);
void clPermutations(ConditionList input, PairList l, CLQueue* outputQueue);
void clSwap(PairList pl1, PairList pl2);
void setFlags(char* flags);
char* subString(char* str, int len);
FILE* setStream(char* arg);
int check_necessary(ConditionList table, ConditionList conditions, int effect);

#endif