#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pairlist.h"

Pair** prepare_table(FILE* input_table, int rows, int cols);
int* step0(Pair** main_table, int* NegFactorSet,int rows, int cols);
int rowDuplicity(Pair** table,int factor,int rows, int cols);
int intArrayIn(int val, int* array);

// int input_table[][5] = {
//                 {1,1,1,1,1},
//                 {1,1,1,0,1},
//                 {1,0,1,1,1},
//                 {0,1,1,1,1},
//                 {0,1,1,0,1},
//                 {1,0,1,0,1},
//                 {0,0,0,1,1},
//                 {0,0,0,0,0},
//                 {0,0,0,0,0}
//                 };

// char* B = ['A','B','C','D','E'];


int main(int argc, char* argv[])
{
    int NegFactorSet[0];
    //get num coincidences and factors from argv:
    int rows = 8;
    int cols = 5;

    //read csv
    FILE* stream = fopen("TESTINPUT.csv", "r");


    Pair** table = prepare_table(stream, rows, cols);

    for (int i = 0; i < rows;i++) {
        for (int j = 0; j < cols;j++) {
            printf("%d", table[i][j]->value);
        }
        printf("\n");
    }

    int* potential_effects = step0(table,NegFactorSet, rows, cols);

    // //--DEBUG
    // printf("Potential Effects: [");
    // for(int i=0;i<sizeof(potential_effects)/sizeof(int);i++)
    // {
    //     printf("%d,",potential_effects[i]);
    // }
    // printf("]\n");
    // //--

}

Pair** prepare_table(FILE* input_table, int rows, int cols)
{
    Pair** table = malloc(sizeof(Pair*) * rows);
    int r = 0;
    int c = 0;
    char line[2];

    table[r] = malloc(sizeof(Pair) * cols);
    while(fgets(line,2,input_table))
    {
        if(line[0] == ','){c++;}
        else if(line[0]=='\n')
        {
            r++;
            c = 0;
            table[r] = malloc(sizeof(Pair) * cols);
        }
        else
        {
            table[r][c] = make_pair(c,atoi(&line[0]));
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
int* step0(Pair** main_table, int* NegFactorSet, int rows, int cols)
{
    int* W = malloc(sizeof(int)*cols);
    int W_index = 0;
    for (int i=0;i<cols;i++)
    {
        printf("checking index %d\n", i);
        if(!(rowDuplicity(main_table, i, rows, cols) || intArrayIn(i, NegFactorSet)))
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
int rowDuplicity(Pair** table,int factor, int rows, int cols)
{
    Pair** rowSet = malloc(sizeof(Pair*) * rows);
    // memset(rowSet, 0, sizeof(*rowSet));
    //loop through every row in table
    for(int rowPos=0;rowPos<rows;rowPos++)
    {
        rowSet[rowPos] = malloc(sizeof(Pair) * cols);
        memset(rowSet, 0, sizeof(Pair)*cols);
        table[rowPos][factor]->value = 1-table[rowPos][factor]->value;
        if (pairListInList(rowSet,table[rowPos], rows, cols))
        {
            table[rowPos][factor]->value = 1-table[rowPos][factor]->value;
            return 1;
        }
        table[rowPos][factor]->value = 1-table[rowPos][factor]->value;
        rowSet[rowPos] = table[rowPos];
        printf("rowpos:%d\n",rowPos);
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
