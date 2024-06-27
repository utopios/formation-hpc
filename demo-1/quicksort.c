#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void quicksort(int *numbers, int left, int right) {
    int pivot, l_hold, r_hold;

    l_hold = left;
    r_hold = right;
    pivot = numbers[left];
    while (left < right) {
        while ((numbers[right] >= pivot) && (left < right))
            right--;
        if (left != right) {
            numbers[left] = numbers[right];
            left++;
        }
        while ((numbers[left] <= pivot) && (left < right))
            left++;
        if (left != right) {
            numbers[right] = numbers[left];
            right--;
        }
    }
    numbers[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    if (left < pivot)
        quicksort(numbers, left, pivot-1);
    if (right > pivot)
        quicksort(numbers, pivot+1, right);
}

int main() {
    int n = 10000000;
    int *numbers = malloc(n * sizeof(int));

    // Generate random numbers
    srand(time(NULL));
    for (int i = 0; i < n; i++) {
        numbers[i] = rand() % n;
    }

    // Measure sorting time
    clock_t start = clock();
    quicksort(numbers, 0, n - 1);
    clock_t end = clock();

    // Output the time taken
    double time_spent = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Time taken to sort %d numbers: %f seconds\n", n, time_spent);

    free(numbers);
    return 0;
}