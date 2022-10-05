#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "omp.h"
#include <stdlib.h>
#include "time.h"
#include "sys/time.h"

#define		DATA_SIZE		1048576

// A utility function to swap two elements 
void swap(float* a, float* b) 
{ 
    float t = *a; 
    *a = *b; 
    *b = t; 
} 
  
int partition (float arr[], int low, int high) 
{ 
    float pivot = arr[high];
    int i = (low - 1);
  
    for (int j = low; j <= high- 1; j++) 
    {  
        if (arr[j] < pivot)
        { 
            i++;
            swap(&arr[i], &arr[j]);
        } 
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
} 

void quickSort(float arr[], int low, int high) 
{ 
    if (low < high) 
    { 
        int pi = partition(arr, low, high); 

        quickSort(arr, low, pi - 1); 
        quickSort(arr, pi + 1, high); 
    } 
}

void mergeSortedArrays(float *arr1, int arr1FirstIdx, int arr1LastIdx, float *arr2, int arr2FirstIdx, int arr2LastIdx, float *result) {
   int i, j, k, n1, n2;
   i = arr1FirstIdx;
   j = arr2FirstIdx;
   k = 0;

   while (i <= arr1LastIdx && j <= arr2LastIdx) {
      if (arr1[i] < arr2[j]) {
         result[k] = arr1[i];
         ++k;
         ++i;
      } else {
         result[k] = arr2[j];
         ++k;
         ++j;
      }
   }
   while (i <= arr1LastIdx) {
      result[k] = arr1[i];
      ++k;
      ++i;
   }
   while (j <= arr2LastIdx) {
      result[k] = arr2[j];
      ++k;
      ++j;
   }
}

/* Function to print an array */
void printArray(float arr[], int size) 
{ 
    int i; 
    for (i=0; i < size; i++) 
        printf("%f ", arr[i]); 
    printf("\n"); 
}

int main(int argc, char * argv[])
{
	struct timeval start, end;
	int i;
    float *p = (float*) malloc(DATA_SIZE * sizeof(float));
    float *s = (float*) malloc(DATA_SIZE * sizeof(float));
    float *sOut1 = (float*) malloc(DATA_SIZE/2 * sizeof(float));
    float *sOut2 = (float*) malloc(DATA_SIZE/2 * sizeof(float));
	// float p[DATA_SIZE], s[DATA_SIZE], sOut1[DATA_SIZE/2], sOut2[DATA_SIZE/2];
    
    srand(time(NULL));

	for (i = 0; i < DATA_SIZE; i++) {
		p[i] = (float)rand() / (float)(RAND_MAX/DATA_SIZE);
		s[i] = p[i];
	}

    // Serial
	gettimeofday(&start, NULL);

	quickSort(s, 0, DATA_SIZE-1);

	gettimeofday(&end, NULL);

	long seconds1 = end.tv_sec - start.tv_sec;
	long micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;
    
    // Parallel
	gettimeofday(&start, NULL);

	#pragma omp parallel num_threads(4) shared(p)
	{
		int threadId = omp_get_thread_num();
		int startIdx = threadId * DATA_SIZE / 4;
		int endIdx = ((threadId+1) * DATA_SIZE / 4) - 1;

		quickSort(p, startIdx, endIdx);
	}
	mergeSortedArrays(p, 0, DATA_SIZE/4 - 1, p, DATA_SIZE/4, DATA_SIZE/4 * 2 - 1, sOut1);
	mergeSortedArrays(p, DATA_SIZE/4 * 2, DATA_SIZE/4 * 3 - 1, p, DATA_SIZE/4 * 3, DATA_SIZE - 1, sOut2);
	mergeSortedArrays(sOut1, 0, DATA_SIZE/2 - 1, sOut2, 0, DATA_SIZE/2 - 1, p);

	gettimeofday(&end, NULL);

	long seconds2 = end.tv_sec - start.tv_sec;
	long micros2 = (seconds2 * 1000000) + end.tv_usec - start.tv_usec;
    
    // Reports
	printf("Kimia Khabiri: 810196606 - Parsa Hoseininejad: 810196604\n");
	printf("Serial Run time = %ld\n", micros1);
	printf("Parallel Run time = %ld\n", micros2);
	printf("Speedup = %f\n", (float) (micros1)/(float) micros2);

    free(s);
    free(p);
    free(sOut1);
    free(sOut2);

	return 0;
}