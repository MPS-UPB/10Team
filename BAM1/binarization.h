#pragma once

#include "Direct_Access_Image.h"

typedef struct Part {
	int y0, y1, x0, x1;
}Part;

/* Compute the histogram for given image */
int *getHistogram(BYTE **img, int y0, int y1, int x0, int x1);

/* Compute threshold with otsu method */
int getOtsuThreshold(int *histData, int y0, int y1, int x0, int x1);

/* Compute number of sharp peaks */
int computeSharpPeak(int *histogram, int nrTotalPixels);

/* Binarization with Otsu method */
int otsuBinarization(KImage *binImg, BYTE **img, int global, int *hist, int y0, int y1, int x0, int x1);

/* Binarization algorithm */
int binarization(KImage *binImg, BYTE **img, int y0, int y1, int x0, int x1);

/* Get PP parameter */
double getPP(int *histogram, int nrTotalPixels);

/* Noise filter */
BYTE **medianFilter(BYTE **img, int iterations, int height, int width);

/* Calculate the confidence matrix */
BYTE getConfidence(BYTE pixel, BYTE global, BYTE threshold);