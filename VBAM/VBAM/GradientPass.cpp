#include "GradientPass.h"
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>


GradientPass::GradientPass(void)
{
	ImagePass::filterName = "Gradient filter";
}


GradientPass::~GradientPass(void)
{
}

void GradientPass::processImage(cv::Mat image, cv::Mat &confidence)
{
	cv::Mat im1;
	cv::Mat im2;
	
	cv::morphologyEx(image, im1, cv::MORPH_CLOSE, cv::Mat());
	cv::morphologyEx(im1, im2, cv::MORPH_GRADIENT, cv::Mat());
	
	//cv::imwrite("ceva8.png", im2);

	//Compari fiecare pixel si cresti/scazi confidenta dupa ce criterii vrei
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			PixelColor color = getPixelColor(image, i, j);
			PixelColor otherColor = getPixelColor(im2, i, j);

			float conf = getConfidence(confidence, i, j);

			if(otherColor == PIXEL_WHITE)
				conf *= 10;

			setConfidence(confidence, i, j, conf);
		}
	}

}