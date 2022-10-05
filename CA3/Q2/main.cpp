#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>
#include "x86intrin.h"
#include "sys/time.h"

using std::cout;
using std::endl;

int main()
{
	struct timeval start, end;

	// Load image
    cv::Mat image1 = cv::imread("1.png", cv::IMREAD_GRAYSCALE);
    cv::Mat image2 = cv::imread("2.png", cv::IMREAD_GRAYSCALE);
	
	unsigned int NCOLS = image1.cols;
	unsigned int NROWS = image1.rows;

	unsigned int NCOLS2 = image2.cols;
	unsigned int NROWS2 = image2.rows;

	unsigned int temp;

	// Create output image
	cv::Mat serialOutImage(image1.rows, image1.cols, CV_8U);
	cv::Mat parallelOutImage(image1.rows, image1.cols, CV_8U);

	// Pointer to the image data (Matrix of pixels)
	unsigned char *image1Ptr;
	unsigned char *image2Ptr;

	image1Ptr = (unsigned char *) image1.data;
	image2Ptr = (unsigned char *) image2.data;

	// Serial
	unsigned char *serialOutImagePtr;
	serialOutImagePtr = (unsigned char *) serialOutImage.data;

	gettimeofday(&start, NULL);

	for (int row = 0; row < NROWS; row++)
		for (int col = 0; col < NCOLS; col++) 
			*(serialOutImagePtr + row * NCOLS + col) = *(image1Ptr + row * NCOLS + col);

	for (int row = 0; row < NROWS2; row++)
		for (int col = 0; col < NCOLS2; col++) {
			temp = *(image1Ptr + row * NCOLS + col) + *(image2Ptr + row * NCOLS2 + col) / 2;
			if (temp > 255)
				temp = 255;
			*(serialOutImagePtr + row * NCOLS + col) = temp;
		}

	gettimeofday(&end, NULL);

	long seconds1 = end.tv_sec - start.tv_sec;
	long micros1 = (seconds1 * 1000000) + end.tv_usec - start.tv_usec;

	// Parallel
	__m128i *pImage1Ptr;
	__m128i *pImage2Ptr;
	__m128i *parallelOutImagePtr;
	__m128i m1, m2, m3, m4, m5, m6;

	pImage1Ptr = (__m128i *) image1.data;
	pImage2Ptr = (__m128i *) image2.data;
	parallelOutImagePtr = (__m128i *) parallelOutImage.data;

	m3 = _mm_set1_epi8(0x7F);

	gettimeofday(&start, NULL);

	for (int i = 0; i < NROWS; i++)
		for (int j = 0; j < NCOLS / 16; j++)
		{
			m1 = _mm_loadu_si128(pImage1Ptr + i * NCOLS/16 + j);
			_mm_storeu_si128(parallelOutImagePtr + i * NCOLS/16 + j, m1);
		}

	for (int i = 0; i < NROWS2; i++)
		for (int j = 0; j < NCOLS2 / 16; j++)
		{
			m1 = _mm_loadu_si128(pImage1Ptr + i * NCOLS/16 + j);
			m2 = _mm_loadu_si128(pImage2Ptr + i * NCOLS2/16 + j);

			m4 = _mm_srli_epi16(m2, 1);
			m5 = _mm_and_si128(m2, m3);
			m6 = _mm_adds_epu8(m1, m5);

			_mm_storeu_si128(parallelOutImagePtr + i * NCOLS/16 + j, m6);
		}
		
	gettimeofday(&end, NULL);

	long seconds2 = end.tv_sec - start.tv_sec;
	long micros2 = (seconds2 * 1000000) + end.tv_usec - start.tv_usec;

	// Reports
	printf("Kimia Khabiri: 810196606 - Parsa Hoseininejad: 810196604\n");
	printf("Serial Run time = %ld\n", micros1);
	printf("Parallel Run time = %ld\n", micros2);
	printf("Speedup = %f\n", (float) (micros1)/(float) micros2);

	// Display
    cv::namedWindow("serial image", cv::WINDOW_AUTOSIZE);
    cv::imshow("serial image", serialOutImage);

    cv::namedWindow("parallel image", cv::WINDOW_AUTOSIZE);
    cv::imshow("parallel image", parallelOutImage);

    cv::waitKey(0);
    return 0;
}