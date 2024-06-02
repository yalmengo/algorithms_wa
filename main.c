#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <emscripten/emscripten.h>
#include "algorithms.c"
#include <time.h>

#define EXTERN

typedef struct {
    int* array;
    int size;
} SortArgs;

typedef struct {
    int* array;
    int size;
    int target;
} SearchArgs;

void printArray(int arr[], int size, char* from)
{
    printf("\n--> Array from %s: ", from);
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void* printBubbleSort(void* args)
{
    SortArgs* sortArgs = (SortArgs*)args;

    int copiedArray[sortArgs->size];
    memcpy(copiedArray, sortArgs->array, sizeof(int) * sortArgs->size);
    printf("\nBubble Sort started...\n");
    bubbleSort(copiedArray, sortArgs->size);
    printArray(copiedArray, sortArgs->size, "Bubble Sort");

    return NULL;
}

void* printQuickSort(void* args)
{
    SortArgs* sortArgs = (SortArgs*)args;
    int copiedArray[sortArgs->size];
    memcpy(copiedArray, sortArgs->array, sizeof(int) * sortArgs->size);
    printf("\nQuick Sort started...\n");
    quickSort(copiedArray, 0, sortArgs->size - 1);
    printArray(copiedArray, sortArgs->size, "Quick Sort");

    return NULL;
}

void* printInsertionSort(void* args)
{
    SortArgs* sortArgs = (SortArgs*)args;
    int copiedArray[sortArgs->size];
    memcpy(copiedArray, sortArgs->array, sizeof(int) * sortArgs->size);
    printf("\nInsertion Sort started...\n");
    insertionSort(copiedArray, sortArgs->size);
    printArray(copiedArray, sortArgs->size, "Insertion Sort");

    return NULL;
}

void* printSequentialSearch(void* args) {
    SearchArgs* searchArgs = (SearchArgs*)args;
    int copiedArray[searchArgs->size];
    memcpy(copiedArray, searchArgs->array, sizeof(int) * searchArgs->size);
    int index = sequentialSearch(copiedArray, searchArgs->size, searchArgs->target);
    if (index != -1) {
        printf("\nElement %d was found at index %d using Sequential Search.\n", searchArgs->target, index);
    } else {
        printf("\nElement %d is not found in the array using Sequential Search.\n", searchArgs->target);
    }

    return NULL;
}

void* printBinarySearch(void* args) {
    SearchArgs* searchArgs = (SearchArgs*)args;
    int copiedArray[searchArgs->size];
    memcpy(copiedArray, searchArgs->array, sizeof(int) * searchArgs->size);
    int index = binarySearch(copiedArray, 0, searchArgs->size - 1, searchArgs->target);
    if (index != -1) {
        printf("\nElement %d was found at index %d using Binary Search.\n", searchArgs->target, index);
    } else {
        printf("\nElement %d is not found in the array using Binary Search.\n", searchArgs->target);
    }

    return NULL;
}

int main()
{
    printf("Waiting for inputs...\n\n");
    return 0;
}

pthread_t createThread(void* (*function)(void*), void* args) {
    pthread_t thread;
    if (pthread_create(&thread, NULL, function, args)) {
        fprintf(stderr, "Error creating thread\n");
        exit(1);
    }
    return thread;
}

// EMSCRIPTEN_KEEPALIVE is to avoid this function from being deleted since it's not being called from main
EXTERN EMSCRIPTEN_KEEPALIVE void run(int size, int array[], int target)
{
    int sortedArray[size];
    memcpy(sortedArray, array, sizeof(int) * size);
    printf("\nSorting array to use it in Search algorithms...\n");
     // Sort array so Binary Search can work correctly, using Quick Sort because it's "faster" 😎
    quickSort(sortedArray, 0, size - 1);
    printArray(sortedArray, size, "Main (this will be passed to search algorithms only)");
    printf("\n...\n");
    printf("\nRunning all Sorting and Search Algorithms in parallel...\n");

    // Start timer
    clock_t start, end;
    double cpu_time_used;
    start = clock();

    // Create threads for algorithms
    pthread_t threads[5];

    SortArgs sortArgs = {array, size};
    SearchArgs searchArgs = {sortedArray, size, target};

    threads[0] = createThread(printBubbleSort, &sortArgs);
    threads[1] = createThread(printQuickSort, &sortArgs);
    threads[2] = createThread(printInsertionSort, &sortArgs);
    threads[3] = createThread(printSequentialSearch, &searchArgs);
    threads[4] = createThread(printBinarySearch, &searchArgs);

    for (int i = 0; i < 5; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("\nAll algorithms finished!\n");

    // End timer
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken: %f seconds\n", cpu_time_used);
}
