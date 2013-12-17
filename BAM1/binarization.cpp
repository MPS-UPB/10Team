#include "stdafx.h"

/*
 * calculate the histogram of the image (number of pixels for each gray value)
 * params
 * @img: image matrix
 * @y0, y1: Oy coordinates of the image
 * @x0, x1: Ox coordinates of the image
 * @return: histogram of the image as an array
*/
int *getHistogram(BYTE **img, int y0, int y1, int x0, int x1) {
	int *histogram = (int*) calloc(256, sizeof(int));

	for(int i = y0; i < y1; i++) {
		for(int j = x0; j < x1; j++) {
			unsigned int h = img[i][j];
			histogram[h] = histogram[h] + 1;
		}
	}

	return histogram;
}


/*
 * calculate a global threshold for the image using Otsu algorithm
 * params
 * @histData: histogram of the image
 * @y0, y1: Oy coordinates of the image
 * @x0, x1: Ox coordinates of the image
 * @return: global threshold for the image
*/
int getOtsuThreshold(int *histData, int y0, int y1, int x0, int x1) {
	int height = y1 - y0;
    int width  = x1 - x0;

	// Total number of pixels
	int total = height * width;

	float sum = 0;
	for (int t = 0; t < 256 ; t++) 
		sum += t * histData[t];

	float sumB = 0;
	int wB = 0;
	int wF = 0;

	float varMax = 0;
	int threshold = 0;

	for (int t = 0 ; t < 256 ; t++) {
		wB += histData[t];              // Weight Background
		if (wB == 0) continue;

		wF = total - wB;                 // Weight Foreground
		if (wF == 0) break;

		sumB += (float) (t * histData[t]);

		float mB = sumB / wB;            // Mean Background
		float mF = (sum - sumB) / wF;    // Mean Foreground

		// Calculate Between Class Variance
		float varBetween = (float)wB * (float)wF * (mB - mF) * (mB - mF);

		// Check if new maximum found
		if (varBetween > varMax) {
			varMax = varBetween;
			threshold = t;
		}
	}

	return threshold;
}

/*
 * compute the confidence for a single pixel
 * params
 * @pixel: pixel from the binarized image
 * @global: a global threshold of the image
 * @threshold: a local threshold for the image containing pixel
 * @return: a value between [0, 255].
 *			- 0: not sure if the pixel for the binarized image is correct
 *			- 255: confident that the pixel in the binarized image is correct
*/
BYTE getConfidence(BYTE pixel, BYTE global, BYTE threshold) {
	BYTE min = 0, max = 150, proc = 0;
	// first case: 0 < global_threshold < local_threshold < 255 
	if(global < threshold) {
		// sure about the pixels from [0, global_threshold]
		if(pixel < global)
			return 255 - pixel;

		// sure about the pixels from [local_threshold, 255]
		if(pixel > threshold)
			return pixel;

		// scale confidence from [global_threshold, local_threshold] to [0, 150] confidence
		if(pixel >= global && pixel <= threshold)
			proc = (pixel - global + 1) * 100 / (threshold - global + 1);

	// second case: 0 < local_threshold < global_threshold < 255 
	} else {
		// sure about the pixels from [0, local_threshold]
		if(pixel < threshold)
			return 255 - pixel;
		
		// sure about the pixels from [global_threshold, 255]
		if(pixel > global) 
			return pixel;
		
		// scale confidence from [local_threshold, global_threshold] to [0, 150] confidence
		if(pixel <= global && pixel >= threshold)
			proc = 100 - (pixel - threshold + 1) * 100 / (global - threshold + 1);
		
	}	
	return (BYTE) min + (max - min) * proc / 100;
}

/*
 * compute Otsu binarization from a image fragment
 * while computing binarization calculate the confidence matrix
 * params
 * @binImg: binarized image
 * @img: original image matrix, replace the original pixel with his confidence
 * @global: threshold for the entire image
 * @hist: histogram for the fragment image
 * @y0, y1: Oy coordinates of the image
 * @x0, x1: Ox coordinates of the image
 * @return: 0 - SUCCESS
 *         -3 - IMAGE ERROR 
*/
int otsuBinarization(KImage *binImg, BYTE **img, int global, int *hist, int y0, int y1, int x0, int x1) {
	if (binImg->BeginDirectAccess()) {
		BYTE threshold = (BYTE) getOtsuThreshold(hist, y0, y1, x0, x1);
		BYTE tmp = threshold; //TODO: modify the name of this variable

		/* if the difference between local and global threshold is greater than 25 
			then use global threshold */
		if(threshold - global > 25)
			threshold = (BYTE) global;
		
        //Apply threshold
        for (int y = y0; y < y1; y++) {
            for (int x = x0; x < x1; x++) {
				BYTE &PixelAtXY = img[y][x];
                if (PixelAtXY < threshold) {
					img[y][x] = (BYTE) getConfidence(img[y][x], (BYTE)global, tmp); 
					binImg->Put1BPPPixel(x, y, false);
				} else {
					img[y][x] = (BYTE) getConfidence(img[y][x], (BYTE)global, tmp);
					binImg->Put1BPPPixel(x, y, true);
				}
			}
		}
        //Close direct access
        binImg->EndDirectAccess();
    } else {
        _tprintf(_T("Unable to obtain direct access in binary image!"));
        return -3;
    }

	return 0;
}

/*
 * compute sharp peaks from the histogram of the image
 * params
 * @histogram: histogram of the image
 * @nrTotalPixels: weigh * height
 * @return:  number of sharp peakss
*/
int computeSharpPeak(int *histogram, int nrTotalPixels) {
	int p1[256], p2[256], n1[256], n2[256];
	int nrPeaks = 0, nrSharpPeaks = 0;
	double freq[256], totalFreqPeak = 0;
	//init
	for(int i = 0; i < 256; i++) {
		//frequency for each shade of grey
		freq[i] = (double)histogram[i] / (double)nrTotalPixels;
		if(i == 0) {
			/* The previous 2 and next 2 neighbor positions for 
				each gray value is taken in a circular manner in
				p1, p2, n1, and n2 respectively 
			*/
			p1[i] = 255; p2[i] = 254; n1[i] = 1; n2[i] = 2;
		} else {
			if(i == 1) {
				p1[i] = 0; p2[i] = 255; n1[i] = 2; n2[i] = 3;
			} else {
				if(i == 254) {
					p1[i] = 253; p2[i] = 252; n1[i] = 255; n2[i] = 0;
				} else {
					if(i == 255) {
						p1[i] = 254; p2[i] = 253; n1[i] = 0; n2[i] = 1;
					} else {
						p1[i] = i - 1; p2[i] = i - 2; n1[i] = i + 1; n2[i] = i + 2;
					}
				}
			}
		}
	}
	/* if the frequency of a gray value k is greater than that of 
		its two adjacent right gray value frequencies and two adjacent left 
		gray value then k is identified as a peak 
	*/
	for(int i = 0; i < 256; i++) {
		double m1 = std::max(freq[p1[i]], freq[p2[i]]);
		double m2 = std::max(freq[n1[i]], freq[n2[i]]);
		
		if(freq[i] > std::max(m1, m2)) {
			nrPeaks++;
			totalFreqPeak += freq[i];
		}
	}
	//average pixel frequency for peaks
	double avgPeakFreq = totalFreqPeak / (double)nrPeaks;
	/* a peak is defined as a Sharp Peak, where the frequency of occurences of
		pixels for the corresponding gray-scale value is greater than the
		average peak frequency, avg-peak, computed above 
	*/
	for(int i = 0; i < 256; i++) {
		if(freq[i] > avgPeakFreq) {
			nrSharpPeaks++;
		}
	}

	return nrSharpPeaks;
}

/*
 * compute partition parameter
 * PP = PR(partition ratio) * K
 * params
 * @histogram: histogram of the image
 * @nrTotalPixels: weigh * height
 * @return: partition parameter
*/
double getPP(int *histogram, int nrTotalPixels) {
	int nrPixelLessAvg = 0;
	double avg = 0, K = 15, PR;
	
	for(int i = 0; i < 256; i++)
		avg += i * histogram[i];

	avg = avg / (double) nrTotalPixels;

	for(int i = 0; i < avg; i++)
		nrPixelLessAvg += histogram[i];
	
	/* partition ratio is defined as the ratio between the number
		of pixels having gray scale value greater than avg and the 
		number of pixels having gray scale value less than avg 
	*/
	PR = (double)(nrTotalPixels - nrPixelLessAvg) / (double)nrPixelLessAvg;

	return K * PR;
}

/*
 * apply binarization algorithm
 * params
 * @binImg: space for binarized image
 * @img: original image
 * @y0, y1: Oy coordinates of the image
 * @x0, x1: Ox coordinates of the image
 * @return: 0 - SUCCESS
*/
int binarization(KImage *binImg, BYTE **img, int y0, int y1, int x0, int x1) {
	std::stack<Part*> stack;
	Part *p = new Part;
	int *hist = getHistogram(img, y0, y1, x0, x1);
	int global = getOtsuThreshold(hist, y0, y1, x0, x1);
	p->x0 = x0; p->x1 = x1; p->y0 = y0; p->y1 = y1;
	free(hist);
	
	stack.push(p);

	while(!stack.empty()) {
		p = stack.top();
		stack.pop();

		int *hist = getHistogram(img, p->y0, p->y1, p->x0, p->x1);
		int rows = p->y1 - p->y0;
		int cols = p->x1 - p->x0;
		int total = rows * cols;

		int n = computeSharpPeak(hist, total);
		double PP = getPP(hist, total);

		/* the original image is binarized here using Otsu's method if the 
			image does not have much variance in gray scale values for pixels 
		*/
		if(n <= 2 || !(rows > PP && cols > PP)) {
			int ret;
			if((ret = otsuBinarization(binImg, img, global, hist, p->y0, p->y1, p->x0, p->x1)) != 0)
				return ret;
		} else {
			// partition the image into four rectangular partitions
			Part *A = new Part;
			Part *B = new Part;
			Part *C = new Part;
			Part *D = new Part;
			int xDiff = p->x1 - p->x0;
			int yDiff = p->y1 - p->y0;
			A->x0 = p->x0;
			A->x1 = p->x0 + xDiff / 2;
			A->y0 = p->y0;
			A->y1 = p->y0 + yDiff / 2;

			B->x0 = p->x0 + xDiff / 2;
			B->x1 = p->x1;
			B->y0 = p->y0;
			B->y1 = p->y0 + yDiff / 2;

			C->x0 = p->x0;
			C->x1 = p->x0 + xDiff / 2;
			C->y0 = p->y0 + yDiff / 2;
			C->y1 = p->y1;

			D->x0 = p->x0 + xDiff / 2;
			D->x1 = p->x1;
			D->y0 = p->y0 + yDiff / 2;
			D->y1 = p->y1;

			stack.push(A);
			stack.push(B);
			stack.push(C);
			stack.push(D);
		}

		free(hist);
	}

	return 0;
}