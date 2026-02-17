#include "diff.h"
#include "abs_max.h"
#include "abs_min.h"
#include <stdlib.h>

int diff(int arr[], int arrLen) {
    return abs_max(arr, arrLen) - abs_min(arr, arrLen);
}