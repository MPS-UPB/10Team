#include "NoiseRemovalPass.h"
#include <opencv2/opencv_modules.hpp>
#include <opencv2/opencv.hpp>



NoiseRemovalPass::NoiseRemovalPass(void)
{
	ImagePass::filterName = "Noise removal filter";
}


NoiseRemovalPass::~NoiseRemovalPass(void)
{
}

void NoiseRemovalPass::processImage(cv::Mat image, cv::Mat &confidence)
{
	cv::Mat dilatedImage;
	cv::Mat erodedImage;
	
	cv::dilate(image, dilatedImage, cv::Mat());
	cv::erode(dilatedImage, erodedImage, cv::Mat());

	//Compari fiecare pixel si cresti/scazi confidenta dupa ce criterii vrei
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			PixelColor color = getPixelColor(image, i, j);
			PixelColor otherColor = getPixelColor(erodedImage, i, j);

			unsigned short conf = getConfidence(confidence, i, j);


			setConfidence(confidence, i, j, conf);
			if(color != otherColor)
				setConfidence(confidence, i, j, 0);

		}
	}

}