#include <stdio.h>
#include <emscripten/emscripten.h>

int main() {
    printf("Waiting for inputs...\n");
    return 0;
}

#define EXTERN

void printArray(int arr[], int size) {
    for (int i = 0; i < size; i++) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

EXTERN EMSCRIPTEN_KEEPALIVE void run(int size, int array[], int target) {
    printArray(array, size);
    printf("Target: %d\n", target);
}
