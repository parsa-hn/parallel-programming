#include 	"stdio.h"
#include 	"sys/time.h"
#include 	"x86intrin.h"
#include 	<math.h>

#define		VECTOR_SIZE		  	160000		

typedef union {
	__m128	 	float128;
	
	float		m128_f32[4];
} floatVec;

void print_spfp_vector(__m128 a) {
	floatVec tmp;
	tmp.float128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%f, ", tmp.m128_f32[i]);
	}
	printf ("%f]\n\n", tmp.m128_f32[0]);
}

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
	for (long i = 0; i < VECTOR_SIZE; i++) {
		fArray[i] = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/100.0)); 
		// printf("Ith random is : %f\n", fArray[i]);
	}

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
	for (long i = 0; i < VECTOR_SIZE; i+=4) {
		vec = _mm_loadu_ps (&fArray[i]);
		unsigned long intArray [4] = {i, i+1, i+2, i+3};
		indexVec = _mm_load_si128((const __m128i*) intArray);
		// print_spfp_vector(vec);
		cmpVec = _mm_cmpgt_ps (maxVec, vec);
		// print_spfp_vector(cmpVec);
		maxVec = _mm_and_ps (maxVec, cmpVec);
		maxIndexVec = _mm_and_ps (maxIndexVec, cmpVec);
		// print_spfp_vector(maxVec);
		vec = _mm_andnot_ps (cmpVec, vec);
		indexVec = _mm_andnot_ps (cmpVec, vec);
		// print_spfp_vector(vec);
		maxVec = _mm_add_ps (maxVec, vec);
		maxIndexVec = _mm_add_ps (maxIndexVec, indexVec);
		// print_spfp_vector(maxVec);
	}
	maxVRes = _mm_cvtss_f32 (maxVec);
	print_spfp_vector(maxVec);

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
