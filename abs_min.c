#include "abs_min.h"
#include <limits.h>
#include <stdlib.h>

int abs_min(int arr[], int arrLen) 
{
    int min = INT_MAX;
    for (int i = 0; i < arrLen; i++) {
        if (abs(arr[i]) < abs(min)) {
            min = arr[i];
        }
    }

    return min;
}