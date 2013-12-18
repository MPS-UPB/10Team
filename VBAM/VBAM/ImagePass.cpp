#include "ImagePass.h"


ImagePass::ImagePass(void)
{
}


ImagePass::~ImagePass(void)
{
}

unsigned short &ImagePass::getConfidence(cv::Mat confidence, int i, int j)
{
	return confidence.at<unsigned short>(i, j);
}

void ImagePass::setConfidence(cv::Mat confidence, int i, int j, unsigned short value)
{
	confidence.at<unsigned short>(i, j) = value;
}

PixelColor ImagePass::getPixelColor(cv::Mat image, int i, int j)
{
	float val = image.at<uchar>(i, j);

	if(val == 0)
		return PIXEL_BLACK;

	return PIXEL_WHITE;
}