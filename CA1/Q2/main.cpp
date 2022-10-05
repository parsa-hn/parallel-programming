#include "stdio.h"
#include "x86intrin.h"

#define U8 0
#define I8 1
#define U16 2
#define I16 3
#define U32 4
#define I32 5
#define U64 6
#define I64 7

typedef union {
	__m128i 			int128;
	
	unsigned char		m128_u8[16];
	signed char			m128_i8[16];

	unsigned short		m128_u16[8];
	signed short		m128_i16[8];

	unsigned int		m128_u32[4];
	signed int			m128_i32[4];
	
	unsigned long long		m128_u64[2];
	signed long long		m128_i64[2];
} intVec;

typedef union {
	__m128	 	float128;
	
	float		m128_f32[4];
} floatVec;

void print_u8 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_u8[i]);
	}
	printf ("%X]\n\n", tmp.m128_u8[0]);
}

void print_i8 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=15; i>0; i--) {
		printf ("%X, ", tmp.m128_i8[i]);
	}
	printf ("%X]\n\n", tmp.m128_i8[0]);
}

void print_u16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_u16[i]);
	}
	printf ("%X]\n\n", tmp.m128_u16[0]);
}

void print_i16 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=7; i>0; i--) {
		printf ("%X, ", tmp.m128_i16[i]);
	}
	printf ("%X]\n\n", tmp.m128_i16[0]);
}

void print_u32 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%X, ", tmp.m128_u32[i]);
	}
	printf ("%X]\n\n", tmp.m128_u32[0]);
}

void print_i32 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%X, ", tmp.m128_i32[i]);
	}
	printf ("%X]\n\n", tmp.m128_i32[0]);
}

void print_u64 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=1; i>0; i--) {
		printf ("%llX, ", tmp.m128_u64[i]);
	}
	printf ("%llX]\n\n", tmp.m128_u64[0]);
}

void print_i64 (__m128i a)
{
	intVec tmp;
	tmp.int128 = a;
	printf ("[");
	for (int i=1; i>0; i--) {
		printf ("%llX, ", tmp.m128_i64[i]);
	}
	printf ("%llX]\n\n", tmp.m128_i64[0]);
}

void print_int_vector(__m128i a, unsigned char type) {
	switch(type) {
		case U8: {
			printf("Unsigned byte: ");
			print_u8(a);
			return;
		}
		case I8: {
			printf("Signed byte: ");
			print_i8(a);
			return;
		}

		case U16: {
			printf("Unsigned word: ");
			print_u16(a);
			return;
		}
		case I16: {
			printf("Signed word: ");
			print_i16(a);
			return;
		}

		case U32: {
			printf("Unsigned double word: ");
			print_u32(a);
			return;
		}
		case I32: {
			printf("Signed double word: ");
			print_i32(a);
			return;
		}

		case U64: {
			printf("Unsigned quad word: ");
			print_u64(a);
			return;
		}
		case I64: {
			printf("Signed quad word: ");
			print_i64(a);
			return;
		}

		default: printf("Wrong type!\n");
	}
}

void print_spfp_vector(__m128 a) {
	floatVec tmp;
	tmp.float128 = a;
	printf ("[");
	for (int i=3; i>0; i--) {
		printf ("%f, ", tmp.m128_f32[i]);
	}
	printf ("%f]\n\n", tmp.m128_f32[0]);
}

unsigned char intArray [16] = {	0X00, 0X11, 0X22, 0X33, 0X44, 0X55, 0X66, 0X77,
								0X88, 0X99, 0XAA, 0XBB, 0XCC, 0XDD, 0XEE, 0XFF };
								
float floatArray [4] = { 1.2, 5.5, -2.32, 0.0002 };

int main(void)
{
	__m128i a;
	a = _mm_load_si128((const __m128i*) intArray);

	print_int_vector(a, U8);
	print_int_vector(a, U16);
	print_int_vector(a, U32);
	print_int_vector(a, U64);

	print_int_vector(a, I8);
	print_int_vector(a, I16);
	print_int_vector(a, I32);
	print_int_vector(a, I64);
	
	__m128 b;
	b = _mm_load_ps((float const*) floatArray);

	print_spfp_vector(b);

	return 0;
}