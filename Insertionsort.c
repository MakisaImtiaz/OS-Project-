#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
#define SIZE 25
#define THREAD_COUNT 4
int arr1[SIZE],arr2[SIZE],arr1s[SIZE];
void printArray(int array[])
{
	int i;
    for (i = 0; i < SIZE; i++)
        printf("%d ",array[i]);
}
//structures to hold function parameters for pthread calls
struct mergeParams {
    int begin;
    int mid;
    int end;
};
struct insertionSortParams {
    int start;
    int end;
};
/**
 * Merges two sorted halves of the unsorted[] array into sorted[]
 *
 * @param begin index of first element in first sublist
 * @param mid index of the first element in the second sublist
 * @param end index of the last element in the second sublist
 */
void *merge(void *args) {
    //unpack parameters
    struct mergeParams *params = (struct mergeParams*) args;
    int begin = params->begin,
        mid = params->mid,
        end = params->end;

    int i = begin, j = mid, tpos = begin;

    while (i < mid && j <= end)
    {
        if (arr1[i] < arr1[j])
            arr1s[tpos++] = arr1[i++];
        else
            arr1s[tpos++] = arr1[j++];
    }

    //still elements left over in first list. copy over
    while (i < mid)
        arr1s[tpos++] = arr1[i++];

    //still elements left over in first list. copy over
    while (j <= end)
        arr1s[tpos++] = arr1[j++];
    return NULL;
}

/**
 * Insertion sort
 *
 * @param index of the first element in unsorted sublist
 * @param index of the last element in unsorted sublist
 */
void *insertionSort(void *args)
{
    struct insertionSortParams *params = (struct insertionSortParams*) args;
    int start = params->start,
    end = params->end;

    int i = start, j, itemToInsert;

    //everything to the left of i is sorted
    while (i <= end)
    {
        itemToInsert = arr1[i]; //a must, or else unsorted[i] gets overwritten when shifting elements

        //since everything in this sequence is sorted,
        //starting from i, and going in reverse order, shift the elements to the right
        //until an element less than unsorted[i] is found
        j = i-1;
        while (j >= start && itemToInsert < arr1[j])
        {
            arr1[j+1] = arr1[j];
            j--;
        }
        //insert the element into s[j]
        arr1[j+1] = itemToInsert;
        i++;
    }
    return NULL;
}
double threaded_insertion_sort() {
	clock_t begin, end, total;
	begin=clock();
    pthread_t threads[THREAD_COUNT];
    printf("\nbefore sorting:\t");
    printArray(arr1);
	int start1 = 0,
        end1 = SIZE/2,
        start2 = end1+1,
        end2 = SIZE-1;
    //2 sorting threads, and 1 merge thread
    pthread_t t[3];
    //prepare sorting params and fire off sorting threads
    struct insertionSortParams sArgs[2];
    sArgs[0].start = start1;
    sArgs[0].end = end1;
    pthread_create(&t[0], NULL, insertionSort, &sArgs[0]); //deal with first sublist
    sArgs[1].start = start2;
    sArgs[1].end = end2;
    pthread_create(&t[1], NULL, insertionSort, &sArgs[1]); //deal with second sublist
    //wait for sorting threads to terminate
    pthread_join(t[0], NULL);
    pthread_join(t[1], NULL);
    //prepare params and fire off merging thread
    struct mergeParams mArgs;
    mArgs.begin = start1;
    mArgs.mid = start2;
    mArgs.end = end2;
    pthread_create(&t[2], NULL, merge, &mArgs); //merge the sublists into sorted[]!
    //wait for merging thread to terminate
    pthread_join(t[2], NULL);   
    end=clock();
     //main thread prints out the sorted array
	printf("\nSorted array: ");
	printArray(arr1s);
	printf("\n");
	total=(double)(end - begin);
	printf("    Displaying time taken using multithreaded Insertion sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin  );
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",total/(double)CLOCKS_PER_SEC);
	return total;
}
double basic_insertion_sort() {
	int i;
    clock_t begin, end, total;
    begin=clock();    
	printf("\nbefore sorting:\t");
    printArray(arr2);
	for (i = 1; i < SIZE; i++) {
	    int key = arr2[i];
	    int j = i - 1;
	
	    // Compare key with each element on the left of it until an element smaller than
	    // it is found.
	    // For descending order, change key<array[j] to key>array[j].
	    while (key < arr2[j] && j >= 0) {
	      arr2[j + 1] = arr2[j];
	      --j;
	    }
	    arr2[j + 1] = key;
  }
  	printf("\nSorted array: ");
	printArray(arr2);
    end=clock();
    total=(double)(end - begin);
    printf("\n    Displaying time taken using basic insertion sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin  );
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",total/(float)CLOCKS_PER_SEC);
    return total;
}
int main(){
	double t1,t2;
	int i;	
	for (i=0 ; i<SIZE; i++){
		arr1[i]=rand()%100;
		arr2[i]=arr1[i];
    }
    printf("\n\t\tProcess\t\t\n");
    t2=basic_insertion_sort();
    printf("\n\n\t\tThreads\t\t\n");
	t1=threaded_insertion_sort();
	
	printf("\n\nComparing the total time taken by multithreaded program and basic program for Insertionsort\n");
	if(t1<t2)
		printf("Multithreaded program is best as it takes less time");
	else if(t2<t1)
		printf("Singlethreaded program is best as it takes less time");
	else
		printf("Both takes equal time!");

}
