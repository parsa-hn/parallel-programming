#include "stdio.h"
#include "unistd.h"
#include "stdlib.h"
#include "omp.h"
#include <stdlib.h>
#include "time.h"
#include "sys/time.h"
#include <math.h> 

#define		DATA_SIZE		1048576


int main(int argc, char * argv[])
{
	struct timeval start, end;
	int i, maxPIdx, maxPIdxLocal, maxSIdx;
	float a[DATA_SIZE];
	float maxP, maxS, maxLocal;
    
    srand(time(NULL));

	for (i = 0; i < DATA_SIZE; i++)
		a[i] = (float)rand() / (float)(RAND_MAX/DATA_SIZE);


    // Serial
	maxS = a[0];
	gettimeofday(&start, NULL);

	for (i = 0; i < DATA_SIZE; i++)
		if (a[i] > maxS) {
            maxS = a[i];
			maxSIdx = i;
		}

	gettimeofday(&end, NULL);

	long seconds1 = end.tv_sec - start.tv_sec;
	long micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;
    
    // Parallel
	maxP = a[0];
	gettimeofday(&start, NULL);

	#pragma omp parallel num_threads(4) shared(a, maxP, maxPIdx) private(maxLocal, maxPIdxLocal)
	{
		maxLocal = a[0];
		#pragma omp for
			for (i = 0; i < DATA_SIZE; i++) {
                if (a[i] > maxLocal) {
				    maxLocal = a[i];
					maxPIdxLocal = i;
				}
            }

		#pragma omp critical (maxP)
		{
            if (maxP < maxLocal) {
                maxP = maxLocal;
				maxPIdx = maxPIdxLocal;
			}
		}
	}

	gettimeofday(&end, NULL);

	long seconds2 = end.tv_sec - start.tv_sec;
	long micros2 = (seconds2 * 1000000) + end.tv_usec - start.tv_usec;
    
    // Reports
	printf("Kimia Khabiri: 810196606 - Parsa Hoseininejad: 810196604\n");
    printf("Parallel output: %f with index: %d, Serial output: %f with index: %d\n", maxP, maxPIdx, maxS, maxSIdx);
	printf("Serial Run time = %ld\n", micros1);
	printf("Parallel Run time = %ld\n", micros2);
	printf("Speedup = %f\n", (float) (micros1)/(float) micros2);

	return 0;
}