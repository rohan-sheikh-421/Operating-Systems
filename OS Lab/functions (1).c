#include <stdio.h>
#include "header.h"

void sort(int array[], int size, int order)
{

    if (order == 1)
    {
        for (int i = 0; i < size; i++)
        {

            for (int j = 0; j < size; j++)
            {

                if (array[i] < array[j])
                {
                    int temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                }
            }
        }
    }
    else
    {

        for (int i = 0; i < size; i++)
        {

            for (int j = 0; j < size; j++)
            {

                if (array[i] > array[j])
                {
                    int temp = array[i];
                    array[i] = array[j];
                    array[j] = temp;
                }
            }
        }
    }
}

void findHighest(int array[], int size, int nth)
{
    sort(array, size, 1);
    printf("%i\n",array[size-nth]);
}


void print(int array[], int size)
{
    for (int i = 0; i < size; i++)
    {
        printf("%i ", array[i]);
    }
    printf("\n");
}