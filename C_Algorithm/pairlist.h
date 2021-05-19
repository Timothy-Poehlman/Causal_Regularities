typedef struct pair_desc *Pair;
struct pair_desc
{
    int index,value;
} pair_desc;

Pair make_pair(int index, int value);
int pairs_equal(Pair p1, Pair p2);
int pairInList(Pair* pairList, Pair pair, int rows, int cols);
int pairListsEqual(Pair* pairList1,Pair* pairList2, int rows, int cols);
int pairListInList(Pair** conditionList, Pair* pairList, int rows, int cols);