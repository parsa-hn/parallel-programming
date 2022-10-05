#include "pthread.h"
#include "stdio.h"
#include "time.h"
#include "sys/time.h"
#include <math.h>

#define NUM_OF_THREADS 4
#define DATA_SIZE 1048576

float a[DATA_SIZE];

typedef struct {
    int tid;
    int startIndex;
    int endIndex;
} in_param_t;

typedef struct {
    float localMax;
    int localMaxIdx;
} out_param_t;

void* findMaximum(void* arg) {
    in_param_t* inp = (in_param_t*) arg;
    int localMaxIdx = inp->startIndex;
    float localMax = a[inp->startIndex];

    for (int i = inp->startIndex; i < inp->endIndex; i++) {
        if (a[i] > localMax) {
            localMax = a[i];
            localMaxIdx = i;
        }
    }

    out_param_t *out_param = (out_param_t*) malloc(sizeof(out_param_t));
    out_param->localMax = localMax;
    out_param->localMaxIdx = localMaxIdx;

    pthread_exit(out_param);
}

int main(void) {
	struct timeval start, end;
    pthread_t th[NUM_OF_THREADS];
    in_param_t in_param[NUM_OF_THREADS] = {
        {0, 0, DATA_SIZE/4},
        {1, DATA_SIZE/4, DATA_SIZE/2},
        {2, DATA_SIZE/2, DATA_SIZE*3/4},
        {3, DATA_SIZE*3/4, DATA_SIZE}
    };
    void *thread_result;
    out_param_t *return_value[NUM_OF_THREADS];

    // initializing
    srand(time(NULL));
	for (int i = 0; i < DATA_SIZE; i++)
		a[i] = (float)rand() / (float)(RAND_MAX/DATA_SIZE);

    // Serial
	float maxS = a[0];
    int maxSIdx = 0;
	
	gettimeofday(&start, NULL);

	for (int i = 0; i < DATA_SIZE; i++)
		if (a[i] > maxS) {
            maxS = a[i];
			maxSIdx = i;
		}

	gettimeofday(&end, NULL);

	long seconds1 = end.tv_sec - start.tv_sec;
	long micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;

    // Parallel
	gettimeofday(&start, NULL);

    // creating threads
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_create(&th[i], NULL, findMaximum, (void*) &in_param[i]);
    }

    // joining threads
    for (int i = 0; i < NUM_OF_THREADS; i++) {
        pthread_join(th[i], &thread_result);
        return_value[i] = (out_param_t*) thread_result;
    }

    // finding maximums
    float maxP = return_value[0]->localMax;
    int maxPIdx = return_value[0]->localMaxIdx;

    for (int i = 1; i < NUM_OF_THREADS; i++) {
        if (return_value[i]->localMax > maxP) {
            maxP = return_value[i]->localMax;
            maxPIdx = return_value[i]->localMaxIdx;
        }
    }

    for (int i = 0; i < NUM_OF_THREADS; i++)
        free(return_value[i]);

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