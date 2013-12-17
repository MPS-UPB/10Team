#pragma once

using namespace cv;

#include "Direct_Access_Image.h"
#include "stdafx.h"

void HaarDecomposeArray(float *A, int width);

void HaarTransposeArray(float *A, int width, int height);

void HaarDecomposeImage(float *A, int width, int height);

void HaarReconstructArray(float *A, int width);

void HaarReconstructImage(float *A, int width, int height);

bool signbit(double arg);

void Denoise(const Mat &input, Mat &output, float threshold);

void haarFilter(Mat &src, Mat &dst);

BYTE **noiseFilter(char *path, int *height, int *width);