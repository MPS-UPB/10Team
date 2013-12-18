#include "ImagePass.h"


ImagePass::ImagePass(void)
{
}


ImagePass::~ImagePass(void)
{
}

float &ImagePass::getConfidence(cv::Mat confidence, int i, int j)
{
	return confidence.at<float>(i, j);
}

void ImagePass::setConfidence(cv::Mat confidence, int i, int j, float value)
{
	confidence.at<float>(i, j) = value;
}

PixelColor ImagePass::getPixelColor(cv::Mat image, int i, int j)
{
	float val = image.at<uchar>(i, j);

	if(val == 0)
		return PIXEL_BLACK;

	return PIXEL_WHITE;
}