#include <stdlib.h>
#include <stdio.h>
#include <time.h>

#define SIZE 1000

int* array_reading();

void bubble_sort(int* arr, int len);

int get_bubble_sort_time(int* arr, int len);

int get_qsort_time(int* arr, int len);

int cmp(const void* a, const void* b);

void print_array(int* arr, int len);

int main() {
    int* arr = array_reading();

    if (arr) {
        int bubble_sort_time = get_bubble_sort_time(arr, SIZE);
        int qsort_time = get_qsort_time(arr, SIZE);

        print_array(arr, SIZE);
        free(arr);

        printf("%d\n", bubble_sort_time);
        printf("%d\n", qsort_time);
    }
}

int* array_reading() {
    int* arr = malloc(SIZE * sizeof(int));

    if (arr) {
        int number = 0;
        int len = 0;

        while (scanf("%d", &number) == 1 && len < SIZE) {
            arr[len++] = number;
        }
    }

    return arr;
}

void bubble_sort(int* arr, int len) {
    for (int i = 0; i < len - 1; i++) {
        for (int j = 0; j < len - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int tmp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = tmp;
            }
        }
    }
}

int get_bubble_sort_time(int* arr, int len) {
    clock_t bubble_start = clock();
    bubble_sort(arr, len);
    clock_t bubble_end = clock();

    return (int) (bubble_end - bubble_start);
}

int get_qsort_time(int* arr, int len) {
    clock_t qsort_start = clock();
    qsort(arr, len, sizeof(int), cmp);
    clock_t qsort_end = clock();

    return (int) (qsort_end - qsort_start);
}

int cmp(const void* a, const void* b) {
    return *(const int*) a - *(const int*) b;
}

void print_array(int* arr, int len) {
    for (int i = 0; i < len; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}
