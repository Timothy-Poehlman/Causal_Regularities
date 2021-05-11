#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pairlist.h"

Pair** prepare_table(int input_table[][5]);
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
                {0,0,0,0,0}
                };

// char* B = ['A','B','C','D','E'];


int main(int argc, char* argv[])
{
    int NegFactorSet[0];
    Pair** table = prepare_table(input_table);
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

Pair** prepare_table(int input_table[][5])
{
    int inputSize = sizeof(input_table)/sizeof(input_table[0]);
    int inputRowSize = sizeof(input_table[0])/sizeof(int);
    Pair** table = malloc( sizeof(int*) * inputSize );

    for(int row=0;row<(inputSize);row++)
    {
        table[row] = malloc( sizeof(Pair) * inputRowSize );
        for(int col=0;col<inputRowSize;col++)
        {
            table[row][col] = make_pair(col,input_table[row][col]);
        }
    }
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
    int Wsize = sizeof(main_table[0])/sizeof(Pair);
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
    int rowPos = 0;
    Pair* modif_coincidence = malloc(sizeof(table[0]));
    //loop through every row in table
    for(int i=0;i<(sizeof(table)/sizeof(table[0]));i++)
    {
        memcpy(modif_coincidence,table[i],sizeof(table[i]));
        modif_coincidence[factor]->value = 1-modif_coincidence[factor]->value;
        if (pairListInList(rowSet,modif_coincidence))
        {
            return 1;
        }
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
