#include "ErosionPass.h"
#include "VBAM.h"


ErosionPass::ErosionPass(void)
{
	ImagePass::filterName = "Erosion filter";
}


ErosionPass::~ErosionPass(void)
{
}

void ErosionPass::processImage(cv::Mat image, cv::Mat &confidence)
{
	cv::Mat erodedImage;
	cv::Mat dilatedImage;
	
	cv::erode(image, erodedImage, cv::Mat());
	cv::dilate(erodedImage, dilatedImage, cv::Mat());
		

	//Compari fiecare pixel si cresti/scazi confidenta dupa ce criterii vrei
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			PixelColor color = getPixelColor(image, i, j);
			PixelColor otherColor = getPixelColor(dilatedImage, i, j);

			unsigned short conf = getConfidence(confidence, i, j);

			if(color == otherColor)
				conf *= 10;
		

			setConfidence(confidence, i, j, conf);
		}
	}

}