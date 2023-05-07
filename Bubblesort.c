#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include<stdlib.h>
#include<unistd.h>
#define MAX 50
#define THREAD_COUNT 4
int arr1[MAX],arr2[MAX];
int swap=0,i;
int rand();
struct position{
    int start;
    int mid;
    int end;
};
void odd(){
    for(i=0;i<MAX-1;i=i+2){
        if(arr1[i]>arr1[i+1]){
            int temp=arr1[i];
            arr1[i]=arr1[i+1];
            arr1[i+1]=temp;
        }
    }
}
void even(){
    for(i=1;i<MAX-1;i=i+2){
        if(arr1[i]>arr1[i+1]){
            int temp=arr1[i];
            arr1[i]=arr1[i+1];
            arr1[i+1]=temp;
        }
    }
}
void printArray(int array[])
{
    for (i = 0; i < MAX; i++)
        printf("%d ",array[i]);
}

void *threaded_bubble_sort(){
	
	pthread_t threads[THREAD_COUNT];
    for(i=0;i<=MAX;++i){
        if(i%2==0)
            pthread_create(&threads[0],NULL,odd,NULL);
        if(2<THREAD_COUNT){
        if(i+4%2==0)
            pthread_create(&threads[2],NULL,even,NULL);
        }
        if(4<THREAD_COUNT){
        if(i+8%2==0)
            pthread_create(&threads[4],NULL,odd,NULL);
        }
        if(i%2!=0)
            pthread_create(&threads[1],NULL,odd,NULL);
        if(2<THREAD_COUNT){
        if(i+4%2!=0)
            pthread_create(&threads[3],NULL,even,NULL);   
        }
        if(4<THREAD_COUNT){
        if(i+8%2!=0)
            pthread_create(&threads[5],NULL,odd,NULL); 
        }
    }
    for(i=0;i<THREAD_COUNT;++i)
    pthread_join(threads[i],NULL);
}
double basic_bubble_sort(){
	int j;
    clock_t begin, end, total;
    begin=clock();    
	printf("\nbefore sorting:\t");
    printArray(arr2);
    for (i = 0; i < MAX-1; ++i) {
    	for (j = 0; j < MAX - i - 1; ++j) {
    		if (arr2[j] > arr2[j + 1]) {
        		int temp = arr2[j];
        		arr2[j] = arr2[j + 1];
        		arr2[j + 1] = temp;
      		}	
    	}
  	}
    printf("\nSorted array: ");
	printArray(arr2);
    end=clock();
    total=(double)(end - begin);
    printf("\n    Displaying time taken using basic bubble sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin  );
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",total/(float)CLOCKS_PER_SEC);
    return total;
}
int main(){
	double t1,t2;	
	for (i=0;i<MAX;i++){
		arr1[i]=rand()%100;
		arr2[i]=arr1[i];
    }
    printf("\n\t\tProcess\t\t\n");
    t2=basic_bubble_sort();
    printf("\n\n\t\tThreads\t\t\n");
    clock_t begin, end;
	begin=clock();
    printf("\nbefore sorting:\t");
    printArray(arr1);
	threaded_bubble_sort();
	printf("\nSorted array: ");
	printArray(arr1);
	printf("\n");
	end=clock();
	t1=(double)(end - begin);
	printf("    Displaying time taken using multithreaded bubble sort    ");
	printf("\nInitial time taken by CPU: %d \n", begin  );
	printf("End time taken by CPU: %d \n", end  );
	printf("Total time taken by the CPU: %f seconds",t1/(double)CLOCKS_PER_SEC);
	
	printf("\n\nComparing the total time taken by multithreaded program and basic program for Bubblesort\n");
	if(t1<t2)
		printf("Multithreaded program is best as it takes less time");
	else if(t1>t2)
		printf("Singlethreaded program is best as it takes less time");
	else
		printf("Both takes equal time!");

}
