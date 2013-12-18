#pragma once

#include <opencv\cv.h>
#include <string>

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

	static unsigned short &getConfidence(cv::Mat confidence, int i, int j);
	static void setConfidence(cv::Mat confidence, int i, int j, unsigned short value);
	static PixelColor getPixelColor(cv::Mat image, int i, int j);

public:
	std::string filterName;
};

