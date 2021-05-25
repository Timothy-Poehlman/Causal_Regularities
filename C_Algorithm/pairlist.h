#ifndef PAIRLIST
#define PAIRLIST
typedef struct pair_desc *Pair;
struct pair_desc
{
    int index,value;
} pair_desc;

typedef struct pair_list *PairList;
struct pair_list
{
    Pair* list;
    int size; //allocated size
    int location; //end of list
    PairList next;
} pair_list;

Pair make_pair(int index, int value);
PairList make_pairList();
PairList copy_pairList(PairList p);
int pairs_equal(Pair p1, Pair p2);
int pairInList(PairList pairList, Pair pair);
int pairListsEqual(PairList pairList1,PairList pairList2);
void pairList_free(PairList l);
void pairList_append(PairList list, Pair p);
void printPairList(PairList pl);
#endif