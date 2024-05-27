#include <stdio.h>

#include "header.h"

void main()
{
    int array[5] = {9,8,5,6,2};
   


    sort(array, 5, 1);

   printf("Sorted Elements: ");
   print(array,5);
   printf("The 3rd highest value in the array is: ");
   findHighest(array,5,3);

}
