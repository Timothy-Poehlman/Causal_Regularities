typedef struct condition_list *ConditionList;
struct condition_list
{
    PairList list;
    int size;
} condition_list;

Pair make_CList();
void CList_append(ConditionList c, PairList p);
void CList_free(ConditionList c);