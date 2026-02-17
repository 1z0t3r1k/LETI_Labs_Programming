#include "abs_max.h"
#include "stdlib.h"

int abs_max(int arr[], int arrLen) 
{
    int max = 0;
    for (int i = 0; i < arrLen; i++) {
        if (abs(arr[i]) > abs(max)) {
            max = arr[i];
        }
    }

    return max;
}