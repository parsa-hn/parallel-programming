#include 	"stdio.h"
#include 	"sys/time.h"
#include 	"x86intrin.h"
#include 	<math.h>

#define		VECTOR_SIZE		  	1048576		

int main (void)
{
	struct timeval start, end;

	float fSTmpRes[4];
	float avgSRes;
	float avgVRes;

	float *fArray;
	fArray = new float [VECTOR_SIZE];

	if (!fArray) {
		printf ("Memory allocation error!!\n");
		return 1;
	}
	// Initialize vectors with random numbers
	for (long i = 0; i < VECTOR_SIZE; i++)
		fArray[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0));

	// Serial implementation
	gettimeofday(&start, NULL);
	fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[0] += fArray[i];
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[1] += fArray[i + 1];
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[2] += fArray[i + 2];
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[3] += fArray[i + 3];
	avgSRes = fSTmpRes[0] + fSTmpRes[1];
	avgSRes += fSTmpRes[2] + fSTmpRes[3];
	avgSRes /= VECTOR_SIZE;
	gettimeofday(&end, NULL);

	long seconds1 = end.tv_sec - start.tv_sec;
	long micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;

	// Parallel implementation
	gettimeofday(&start, NULL);
	__m128 vec;
	__m128 sum = _mm_set1_ps(0.0f);
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		sum = _mm_add_ps (sum, vec);
	}
	sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	avgVRes = _mm_cvtss_f32 (sum);

	avgVRes /= VECTOR_SIZE;

	gettimeofday(&end, NULL);
	long seconds2 = end.tv_sec - start.tv_sec;
	long micros2 = (seconds2 * 1000000) + end.tv_usec - start.tv_usec;

	printf("Kimia Khabiri: 810196606 - Parsa Hoseininejad: 810196604\n");
	printf("\nSerial Average Result   = %f \nParallel Average Result = %f\n", avgSRes, avgVRes);
	printf("Serial Run time = %ld\n", micros1);
	printf("Parallel Run time = %ld\n", micros2);
	printf("Speedup = %f\n\n", (float) (micros1)/(float) micros2);





	float stdSRes;
	float stdVRes;

	// Serial implementation
	gettimeofday(&start, NULL);
	fSTmpRes[0] = fSTmpRes[1] = fSTmpRes[2] = fSTmpRes[3] = 0.0;
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[0] += (fArray[i] - avgSRes) * (fArray[i] - avgSRes);
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[1] += (fArray[i + 1] - avgSRes) * (fArray[i + 1] - avgSRes);
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[2] += (fArray[i + 2] - avgSRes) * (fArray[i + 2] - avgSRes);
	for (long i = 0; i < VECTOR_SIZE; i+=4)
		fSTmpRes[3] += (fArray[i + 3] - avgSRes) * (fArray[i + 3] - avgSRes);
	stdSRes = fSTmpRes[0] + fSTmpRes[1];
	stdSRes += fSTmpRes[2] + fSTmpRes[3];
	stdSRes = sqrt(stdSRes / VECTOR_SIZE);
	gettimeofday(&end, NULL);

	seconds1 = end.tv_sec - start.tv_sec;
	micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;

	// Parallel implementation
	gettimeofday(&start, NULL);
	sum = _mm_set1_ps(0.0f);
	__m128 avgVec = _mm_set1_ps(avgSRes);

	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		vec = _mm_sub_ps (vec, avgVec);
		vec = _mm_mul_ps (vec, vec);
		sum = _mm_add_ps (sum, vec);
	}

	sum = _mm_hadd_ps (sum, sum);
	sum = _mm_hadd_ps (sum, sum);
	stdVRes = _mm_cvtss_f32 (sum);

	stdVRes = sqrt(stdVRes / VECTOR_SIZE);

	gettimeofday(&end, NULL);
	seconds2 = end.tv_sec - start.tv_sec;
	micros2 = (seconds2 * 1000000) + end.tv_usec - start.tv_usec;

	printf("Serial STD Result   = %f \nParallel STD Result = %f\n", stdSRes, stdVRes);
	printf("Serial Run time = %ld\n", micros1);
	printf("Parallel Run time = %ld\n", micros2);
	printf("Speedup = %f\n", (float) (micros1)/(float) micros2);


	return 0;
}
