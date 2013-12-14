#pragma once

#include <opencv\cv.h>

enum PixelColor
{
	PIXEL_BLACK,
	PIXEL_WHITE
};

class ImagePass
{

public:
	ImagePass(void);
	~ImagePass(void);

	virtual void processImage(cv::Mat image, cv::Mat &confidence) = 0;

	static void addValue(uchar &var, int val);
	static uchar &getConfidence(cv::Mat confidence, int i, int j);
	static PixelColor getPixelColor(cv::Mat image, int i, int j);
};

