#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include "abs_max.h"
#include "abs_min.h"
#include "diff.h"
#include "sum.h"

#define SIZE 100

enum operationType {
    ABS_MAX = 0,
    ABS_MIN = 1,
    DIFF = 2,
    SUM = 3
};

int menu() 
{
    int number;
    int arr[SIZE];
    int arrLen = 0;

    scanf("%d", &number);

    while (arrLen < SIZE && scanf("%d", &arr[arrLen]) == 1) {
        arrLen++;
    }

    switch(number) {
        case ABS_MAX:
            printf("%d\n", abs_max(arr, arrLen));
            break;
        case ABS_MIN:
            printf("%d\n", abs_min(arr, arrLen));
            break;
        case DIFF:
            printf("%d\n", diff(arr, arrLen));
            break;
        case SUM:
            printf("%d\n", sum(arr, arrLen));
            break;
        default:
            printf("Данные некорректны\n");
            break;
    }

    return 0;
}