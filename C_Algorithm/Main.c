#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pairlist.h"

Pair** prepare_table(int** input_table);
int* step0(Pair** main_table, int* NegFactorSet);
int rowDuplicity(Pair** table,int factor);
int intArrayIn(int val, int* array);

int input_table[][5] = {
                {1,1,1,1,1},
                {1,1,1,0,1},
                {1,0,1,1,1},
                {0,1,1,1,1},
                {0,1,1,0,1},
                {1,0,1,0,1},
                {0,0,0,1,1},
                {0,0,0,0,0},
                {0,0,0,0,0}
                };

// char* B = ['A','B','C','D','E'];


int main(int argc, char* argv[])
{
    int NegFactorSet[0];
    int rows = 9;
    int cols = 5;
    Pair** table = prepare_table(input_table, rows, cols);

    for (int i = 0; i < 9;i++) {
        for (int j = 0; j < 5;j++) {
            printf("%d", table[i][j]->value);
        }
        printf("\n");
    }

    int* potential_effects = step0(table,NegFactorSet);

    //--DEBUG
    printf("Potential Effects: [");
    for(int i=0;i<sizeof(potential_effects)/sizeof(int);i++)
    {
        printf("%d,",potential_effects[i]);
    }
    printf("]\n");
    //--

}

Pair** prepare_table(int** input_table, int rows, int cols)
{
    Pair** table = malloc( sizeof(Pair*) * rows );

    for(int r=0;r<rows;r++)
    {
        table[r] = malloc( sizeof(Pair) * cols );
        for(int c=0;c<cols;c++)
        {
            table[r][c] = make_pair(c,input_table[r][c]);
        }
    }
    return table;
}

/*
 * STEP 0
 * FactorTable is a hashtable/dictionary of all factors, factorA -> index | key -> value
 * For everything in W, we will have to test this
 * Over every factor, we are collecting the set of factors that are possible effects, this set is W
 * B: set of all factors, main_table: C
 */
int* step0(Pair** main_table, int* NegFactorSet)
{
    int Wsize = sizeof(main_table[0]);
    int* W = malloc(Wsize);
    int W_index = 0;
    for (int i=0;i<Wsize;i++)
    {
        if(!(rowDuplicity(main_table, i) || intArrayIn(i, NegFactorSet)))
        {
            W[W_index] = i;
            W_index++;
        }
    }
    return W;
}

/*
    Check if the same row but negative factor already exists, if not, insert into set, else return 1
    int[i][j] table, i= coincidence, j = factor index
*/
int rowDuplicity(Pair** table,int factor)
{
    Pair** rowSet = malloc(sizeof(table));
    memset(rowSet, 0, sizeof(*rowSet));
    // memcpy(rowset,malloc(sizeof(table[0])))
    int rowPos = 0;
    //loop through every row in table
    for(int i=0;i<(sizeof(table));i++)
    {
        table[i][factor]->value = 1-table[i][factor]->value;
        if (pairListInList(rowSet,table[i]))
        {
            table[i][factor]->value = 1-table[i][factor]->value;
            return 1;
        }
        table[i][factor]->value = 1-table[i][factor]->value;
        rowSet[rowPos] = table[i];
        rowPos++;
    }
    return 0;
}

//Checks if a given int is in an array
int intArrayIn(int val, int* array)
{
    for(int i=0;i<sizeof(array);i++)
    {
        if(array[i] == val)
        {
            return 1;
        }
    }
    return 0;
}
