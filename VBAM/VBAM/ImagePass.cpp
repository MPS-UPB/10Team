#include "ImagePass.h"


ImagePass::ImagePass(void)
{
}


ImagePass::~ImagePass(void)
{
}

void ImagePass::addValue(uchar &var, int val)
{
	int a = (int)var + val;

	a = a < 0? 0 : a;
	a = a > 255 ? 255 : a;

	var = (uchar)a;
}

uchar &ImagePass::getConfidence(cv::Mat confidence, int i, int j)
{
	return confidence.at<uchar>(i, j);
}

PixelColor ImagePass::getPixelColor(cv::Mat image, int i, int j)
{
	uchar val = image.at<uchar>(i, j);

	if(val == 0)
		return PIXEL_BLACK;

	return PIXEL_WHITE;
}