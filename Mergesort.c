#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include<stdlib.h>
#include<unistd.h>
#define arrSize 200
#define THREAD_COUNT 4//depending upon the number of cores of your system but no of threads should preferably be less than or equal to the no of cores
int rand();
int arr1[arrSize] ,arr2[arrSize];
struct pos{
    int temp;
};
void printArray(int array[])
{ 
	int i;
    for (i = 0; i < arrSize; i++)
        printf("%d ",array[i]);
}
void merge(int array[],int low,int mid,int high)
{
	int n1=mid-low+1,n2=high-mid,i,j;
    int left[n1];
    int right[n2];
	for (i=0;i<n1;i++)
		left[i]=array[i+low];
	for(i=0;i<n2;i++)
		right[i]=array[i+mid+1];
	int k=low;
	i=j=0;
	while(i<n1&&j<n2) {
		if(left[i]<=right[j])
			array[k++]=left[i++];
		else
			array[k++]=right[j++];
	}
	while(i<n1) {
		array[k++]=left[i++];
	}
	while(j<n2) {
		array[k++]=right[j++];
	}
}
void sort(int arr[],int low, int high)//10,12
{
	int mid=low+(high-low)/2;//11
	if (low<high) {
		sort(arr,low,mid);//10,11
		sort(arr,mid+1,high);
		merge(arr,low,mid,high);//10,11,12
	}
}
void* merge_sort_threads(void *p)
{
    //lets decide which thread performs sorting on current sub array
	int *thread_count=(int *)p;//1,2
	int low=*(thread_count)*(arrSize/2);//5,10
	int high =(*(thread_count)+1)*(arrSize/2)-1;//9,14
	int mid=low+(high-low)/2;//7,12
    *thread_count=*thread_count+1;//will continue until 3rd
	if (low<high) {
		sort(arr1,low, mid);
		sort(arr1,mid + 1, high);
		merge(arr1,low, mid, high);
	}
}
double threaded_merge_sort()
{
	clock_t begin, end, total;
	int i;
	begin=clock();
	pthread_t threads[THREAD_COUNT];
	struct pos p;
    p.temp=0;
    printf("\nbefore sorting:\t");
    printArray(arr1);
	for (i=0;i<THREAD_COUNT;i++)
		pthread_create(&threads[i],NULL,merge_sort_threads,&p.temp);
	for (i=0;i<THREAD_COUNT;i++)
		pthread_join(threads[i],NULL);
	merge(arr1,0,(arrSize/2-1)/2,arrSize/2-1);
	merge(arr1,arrSize/2,arrSize/2+(arrSize-1-arrSize/2)/2,arrSize-1);
	merge(arr1,0,(arrSize - 1)/2,arrSize-1);
	end=clock();
	printf("\nSorted array: ");
	printArray(arr1);
	printf("\n");
	total=(double)(end - begin);
	printf("    Displaying time taken using multithreaded merge sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin  );
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",total/(double)CLOCKS_PER_SEC);
	return total;
}
double basic_merge_sort(){
    clock_t begin, end, total;
    begin=clock();    
	printf("\nbefore sorting:\t");
    printArray(arr2);
    sort(arr2,0, arrSize - 1);
    printf("\nSorted array: ");
	printArray(arr2);
    end=clock();
    total=(double)(end - begin);
    printf("\n    Displaying time taken using basic merge sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin  );
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",total/(float)CLOCKS_PER_SEC);
    return total;
}
int main(){
    double t1,t2;
	int i;	
	for (i=0;i<arrSize;i++){
		arr1[i]=rand()%100;
		arr2[i]=arr1[i];
    }
    printf("\n\t\tProcess\t\t\n");
    t1=basic_merge_sort();
    printf("\n\n\t\tThreads\t\t\n");
	t2=threaded_merge_sort();
	
	printf("\n\nComparing the total time taken by multithreaded program and basic program for mergesort\n");
	if(t2<t1)
		printf("Multithreaded program is best as it takes less time");
	else if(t1<t2)
		printf("Singlethreaded program is best as it takes less time");
	else
		printf("Both takes equal time!");
	
}
