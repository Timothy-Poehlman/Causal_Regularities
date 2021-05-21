typedef struct pair_desc *Pair;
struct pair_desc
{
    int index,value;
} pair_desc;

typedef struct pair_list *PairList;
struct pair_list
{
    Pair* list;
    int size;
    PairList next;
} pair_list;

typedef struct condition_list *ConditionList;
struct condition_list
{
    PairList list;
    int size;
} condition_list;

Pair make_pair(int index, int value);
int pairs_equal(Pair p1, Pair p2);
int pairInList(Pair* pairList, Pair pair, int rows, int cols);
int pairListsEqual(Pair* pairList1,Pair* pairList2, int rows, int cols);
int pairListInList(Pair** conditionList, Pair* pairList, int rows, int cols);