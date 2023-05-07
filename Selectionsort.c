#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define ARRAY_SIZE 25
#define NUM_THREADS 4

int array[ARRAY_SIZE];
int array1[ARRAY_SIZE];
void printArray(int array[])
{
	int i;
    for (i = 0; i < ARRAY_SIZE; i++)
        printf("%d ",array[i]);
}
double selectionSort(int arr[], int n)
{
	clock_t start,end;
	double t1;
	start = clock();
    printf("\nBefore sortying:");
    printArray(array);
    int i, j, index;
    for (i = 0; i < n - 1; i++)
    {
        index = i;
        for (j = i + 1; j < n; j++)
        {
            if (arr[j] < arr[index])
            {
                index = j;
            }
        }
        int temp = arr[index];
        arr[index] = arr[i];
        arr[i] = temp;
    }
    printf("\nSorted array: ");
    printArray(array);
    end = clock();
    t1 = (end - start) / (double)CLOCKS_PER_SEC;
    printf("\n    Displaying time taken using basic selection sort    ");
	printf("\nInitial time taken by CPU: %d \n", start);
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",t1/(float)CLOCKS_PER_SEC);
	return t1;
}

void *selectionSortThread(void *arg)
{
    int thread_id = *(int *)arg;
    int start = thread_id * (ARRAY_SIZE / NUM_THREADS);
    int end = start + (ARRAY_SIZE / NUM_THREADS);
    int min_index,i,j;

    for (i = start; i < end - 1; i++)
    {
        min_index = i;
        for (j = i + 1; j < end; j++)
        {
            if (array1[j] < array1[min_index])
            {
                min_index = j;
            }
        }
        int temp = array1[i];
        array1[i] = array1[min_index];
        array1[min_index] = temp;
    }

    pthread_exit(NULL);
}
int main()
{
    double t1, t2;
    int i;
    clock_t start,end;
    // Initialize array with random values
    for (i = 0; i < ARRAY_SIZE; i++)
    {
        array[i] = rand() % 100;
        array1[i] = array[i];
    }
    // Process
    printf("\n\t\tProcess\t\t\n");
    t1=selectionSort(array, ARRAY_SIZE);
    // Create threads
    printf("\n\n\t\tThreads\t\t\n");
    start=clock();
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    printf("\nBefore sortying:");
    printArray(array1);
    for (i = 0; i < NUM_THREADS; i++)
    {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, selectionSortThread, (void *)&thread_ids[i]);
    }
    // Join threads
    for (i = 0; i < NUM_THREADS; i++)
    {
        pthread_join(threads[i], NULL);
    }
    // Merge sorted subarrays
    for (i = 1; i < NUM_THREADS; i++)
    {
        int j = i - 1;
        int key = array1[i * (ARRAY_SIZE / NUM_THREADS)];
        while (j >= 0 && array1[j * (ARRAY_SIZE / NUM_THREADS)] > key)
        {
            array1[(j + 1) * (ARRAY_SIZE / NUM_THREADS)] = array1[j * (ARRAY_SIZE / NUM_THREADS)];
            j--;
        }
        array1[(j + 1) * (ARRAY_SIZE / NUM_THREADS)] = key;
	}
	printf("\nSorted array: ");
	printArray(array1);
    end = clock();
    t2 = (end-start) / (double)CLOCKS_PER_SEC;
    printf("\n    Displaying time taken using threaded selection sort    ");
	printf("\nInitial time taken by CPU: %d \n", start);
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",t2/(float)CLOCKS_PER_SEC);
	printf("\n\nComparing the total time taken by multithreaded program and basic program for Selectionsort\n");
	if(t2<t1)
		printf("Multithreaded program is best as it takes less time");
	else if(t1<t2)
		printf("Singlethreaded program is best as it takes less time");
	else
		printf("Both takes equal time!");
    return 0;
}

