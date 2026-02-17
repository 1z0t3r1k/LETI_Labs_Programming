#include "sum.h"
#include "abs_max.h"
#include <stdlib.h>

int sum(int arr[], int arrLen) {
    int maxIndex = 0;
    int finalSum = 0;
    int maxValue = abs_max(arr, arrLen); // cycle optimization
    int isMaxIndexFound = 0;

    for (int i = 0; i < arrLen; i++) {
        if (arr[i] == maxValue && isMaxIndexFound == 0) {
            maxIndex = i;
            isMaxIndexFound = 1;
        }
    }

    for (int i = maxIndex; i < arrLen; i++) {
        finalSum += arr[i];
    }

    return finalSum;
}