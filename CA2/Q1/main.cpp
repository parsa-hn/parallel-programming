#include 	"stdio.h"
#include 	"sys/time.h"
#include 	"x86intrin.h"
#include 	<math.h>

#define		VECTOR_SIZE		  	1048576		

typedef union {
	__m128	 	float128;
	
	float		m128_f32[4];
} floatVec;

int main (void)
{
	struct timeval start, end;

	float fSTmpRes[4];
	float maxSRes;
	float maxVRes;
	int maxIdxSRes;
	int maxIdxVRes;

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
	float max = fArray[0];
	long maxIdx = 0;
	for (long i = 0; i < VECTOR_SIZE; i+=1) {
		if (fArray[i] > max) {
			max = fArray[i];
			maxIdx = i;
		}
	}
	maxSRes = max;
	maxIdxSRes = maxIdx;
	gettimeofday(&end, NULL);

	long seconds1 = end.tv_sec - start.tv_sec;
	long micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;

	// Parallel implementation
	gettimeofday(&start, NULL);
	__m128 vec;
	__m128 indexVec;
	__m128 maxVec = _mm_set1_ps(fArray[0]);
	__m128 cmpVec = _mm_set1_ps(0.0f);
	__m128 maxIndexVec = _mm_set1_ps(0.0f);
	for (unsigned long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		float intArray [4] = {float(i), float(i+1), float(i+2), float(i+3)};
		indexVec = _mm_load_ps(intArray);
		cmpVec = _mm_cmpgt_ps (maxVec, vec);
		maxVec = _mm_and_ps (maxVec, cmpVec);
		maxIndexVec = _mm_and_ps (maxIndexVec, cmpVec);
		vec = _mm_andnot_ps (cmpVec, vec);
		indexVec = _mm_andnot_ps (cmpVec, indexVec);
		maxVec = _mm_add_ps (maxVec, vec);
		maxIndexVec = _mm_add_ps (maxIndexVec, indexVec);
	}

	floatVec fourMaxValues, fourMaxIndexes;
	fourMaxValues.float128 = maxVec;
	fourMaxIndexes.float128 = maxIndexVec;
	maxVRes = fourMaxValues.m128_f32[0];
	float maxIndex = fourMaxIndexes.m128_f32[0];
	for (int i=3; i>0; i--) {
		if(fourMaxValues.m128_f32[i] > maxVRes){
			maxVRes = fourMaxValues.m128_f32[i];
			maxIndex = fourMaxIndexes.m128_f32[i];
		}
	}
	maxIdxVRes = (int)maxIndex;

	gettimeofday(&end, NULL);
	long seconds2 = end.tv_sec - start.tv_sec;
	long micros2 = (seconds2 * 1000000) + end.tv_usec - start.tv_usec;

	printf("Kimia Khabiri: 810196606 - Parsa Hoseininejad: 810196604\n");
	printf("\nSerial Maximum Result   = %f with index %d \n", maxSRes, maxIdxSRes);
	printf("Parallel Maximum Result = %f with index %d\n", maxVRes, maxIdxVRes);
	printf("Serial Run time = %ld\n", micros1);
	printf("Parallel Run time = %ld\n", micros2);
	printf("Speedup = %f\n\n", (float) (micros1)/(float) micros2);

	return 0;
}
