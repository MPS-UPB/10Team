#include "stdafx.h"

using namespace cv;
using namespace std;

void HaarDecomposeArray(float *A, int width) {
    const float inv_sqrt2 = 1 / (float)sqrt(2);

    float norm = 1.0f / (float)sqrt(width);

    for(int i=0; i < width; i++) {
        A[i] *= norm;
    }

    float *tmp = new float[width];

    while(width > 1) {
        width /= 2;

        for(int i=0; i < width; i++) {
            tmp[i] = (A[2*i] + A[2*i+1]) * inv_sqrt2;
            tmp[width + i] = (A[2*i] - A[2*i+1]) * inv_sqrt2;
        }

        memcpy(A, tmp, width*2*sizeof(float));
    }

    delete [] tmp;
}

void HaarTransposeArray(float *A, int width, int height) {
    float *B = new float[width*height];

    for(int y=0; y < height; y++) {
        for(int x=0; x < width; x++) {
            B[x*height + y] = A[y*width + x];
        }
    }

    memcpy(A, B, sizeof(float)*width*height);

    delete [] B;
}

void HaarDecomposeImage(float *A, int width, int height) {
    for(int i=0; i < height; i++) {
        HaarDecomposeArray(&A[i*width], width);
    }

    HaarTransposeArray(A, width, height);

    for(int i=0; i < width; i++) {
        HaarDecomposeArray(&A[i*height], height);
    }

    HaarTransposeArray(A, height, width);
}

void HaarReconstructArray(float *A, int width) {
    const float inv_sqrt2 = 1 / (float)sqrt(2);
    float inv_norm = (float) sqrt(width);

    float *tmp = new float[width];
    int k = 1;

    while(k < width)  {
        for(int i=0; i < k; i++) {
            tmp[2*i] = (A[i] + A[k+i]) * inv_sqrt2;
            tmp[2*i+1] = (A[i] - A[k+i]) * inv_sqrt2;
        }

        memcpy(A, tmp, sizeof(float)*(k*2));

        k *= 2;
    }

    for(int i=0; i < width; i++) {
        A[i] *= inv_norm;
    }

    delete [] tmp;
}

void HaarReconstructImage(float *A, int width, int height) {
    for(int i=0; i < width; i++) {
        HaarReconstructArray(&A[i*height], height);
    }

    HaarTransposeArray(A, height, width);

    for(int i=0; i < height; i++) {
        HaarReconstructArray(&A[i*width], width);
    }

    HaarTransposeArray(A, width, height);
}

bool signbit(double arg) {
	return arg < 0;
}

void Denoise(const Mat &input, Mat &output, float threshold) {
    assert(input.data != output.data);

    input.convertTo(output, CV_32F);

    HaarDecomposeImage((float*)output.data, output.cols, output.rows);

    for(int y=0; y < output.rows; y++) {
        float *ptr = (float*)output.ptr(y);

        for(int x=0; x < output.cols; x++) {
            // signbit returns 1 for negative numbers, and 0 otherwise
            // Haar wavelet shrinkage using soft thresholding
            ptr[x] = (signbit(ptr[x]) == 1 ? -1 : 1) * max(0.f, fabs(ptr[x]) - threshold);
        }
    }

    HaarReconstructImage((float*)output.data, output.rows, output.cols);

    output.convertTo(output, CV_8U);
}

void haarFilter(Mat &src, Mat &dst) {
    assert(src.data);
    // Haar Wavelet requires image to be a power of two
    // We'll pad the borders and remove them later
	Mat g_padded_img, g_denoised_img;
    int padded_width = 0;
    int padded_height = 0;

    padded_width = (int)pow(2, ceil(log(src.cols)/log(2)));
    padded_height = (int)pow(2, ceil(log(src.rows)/log(2)));

	copyMakeBorder(src, g_padded_img, 0, padded_height-src.rows, 0, padded_width-src.cols, BORDER_CONSTANT);

	double g_threshold = 0.02;
	if(g_padded_img.channels() == 3) {
	    Mat bgr[3];
        split(g_padded_img, bgr);

        Denoise(bgr[0].clone(), bgr[0], (float)g_threshold);
        Denoise(bgr[1].clone(), bgr[1], (float)g_threshold);
        Denoise(bgr[2].clone(), bgr[2], (float)g_threshold);

        merge(bgr, 3, g_denoised_img);
    } else {
        Denoise(g_padded_img, g_denoised_img, (float)g_threshold);
    }

    dst = g_denoised_img(Range(0, src.rows), Range(0, src.cols));
}



/*
 * apply noise filters before binarization
 * params
 * @path: path to original image
 * @height
 * @width
 * @return: denoised matrix of the image
 *			null (error)
*/
BYTE **noiseFilter(char *path, int *height, int *width) {
	Mat src = imread(path, CV_LOAD_IMAGE_UNCHANGED);
	Mat dst;
	
	if(src.data == NULL)
		return NULL;
	
	*height = src.rows;
	*width  = src.cols;

	BYTE **img = new BYTE*[src.rows];
	for(int i = 0; i < src.rows; i++)
		img[i] = new BYTE[src.cols];
	
	//// salt and pepper noise
	bilateralFilter(src, dst, 21, 21, 21);
	
	//// blur noise
	GaussianBlur(dst, src, Size(0, 0), 3);

	//// adjust contrast 
	addWeighted(dst, 1.5, src, -0.5, 0, src);
	cvtColor(src, dst, CV_BGR2GRAY);
	
	for(int i = 0; i < dst.rows; i++) {
		for(int j = 0; j < dst.cols; j++) {
			img[i][j] = (BYTE) dst.data[i * dst.cols + j];
		}
	}

	return img;
}