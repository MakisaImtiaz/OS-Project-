#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define MAX_SIZE 200

int array[MAX_SIZE];
int array1[MAX_SIZE];
int num_threads = 4;
pthread_mutex_t lock;
void printArray(int array[])
{
	int i;
    for (i = 0; i < MAX_SIZE; i++)
        printf("%d ",array[i]);
}
// Function to swap two elements
void swap(int *a, int *b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int arr[], int low, int high)
{
    int j, pivot = arr[high];
    int i = low - 1;

    for (j = low; j < high; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }

    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {

        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);

        quickSort(arr, pi + 1, high);
    }
}

void *quickSortThread(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);

        pthread_t threads[2];

        if (num_threads <= 4)
        {
            quickSortThread(arr, low, pi - 1);
            quickSortThread(arr, pi + 1, high);
        }
        else
        {
            pthread_mutex_lock(&lock);

            num_threads--;

            pthread_mutex_unlock(&lock);

            pthread_create(&threads[0], NULL, quickSortThread, pi - 1);
            pthread_create(&threads[1], NULL, quickSortThread, pi + 1);

            pthread_join(threads[0], NULL);
            pthread_join(threads[1], NULL);

            pthread_mutex_lock(&lock);

            num_threads++;

            pthread_mutex_unlock(&lock);
        }
    }
}

int main()
{   int i;
    double t1, t2;
    clock_t begin, end;

    for (i = 0; i < MAX_SIZE; i++)
    {
        array[i] = rand() % 100;
        array1[i] = array[i];
    }

    // Perform Quick Sort
    printf("\n\t\tProcess\t\t\n");
    begin = clock();
    printf("\nBefore sortying:");
    printArray(array);
    quickSort(array, 0, MAX_SIZE - 1);
    printf("\nSorted array: ");
	printArray(array);
    end = clock();
    t1 = (end - begin) / (double)CLOCKS_PER_SEC;
    printf("\n    Displaying time taken using basic quick sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin);
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",t1/(float)CLOCKS_PER_SEC);

   
    // Perform Quick Sort thread
    printf("\n\n\t\tThreads\t\t\n");
    begin = clock();
	printf("\nBefore sortying:");
    printArray(array1);
    pthread_mutex_init(&lock, NULL);
    quickSortThread(array1, 0, MAX_SIZE - 1);
    printf("\nSorted array: ");
	printArray(array1);
    end = clock();
    t2 = (end - begin) / (double)CLOCKS_PER_SEC;
    printf("\n    Displaying time taken using threaded quicksort    ");
	printf("\nInitial time taken by CPU: %d \n", begin);
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",t2/(float)CLOCKS_PER_SEC);
    
   	printf("\n\nComparing the total time taken by multithreaded program and basic program for Quicksort\n");
	if(t2<t1)
		printf("Multithreaded program is best as it takes less time");
	else if(t1<t2)
		printf("Singlethreaded program is best as it takes less time");
	else
		printf("Both takes equal time!");
    return 0;

    return 0;
}
