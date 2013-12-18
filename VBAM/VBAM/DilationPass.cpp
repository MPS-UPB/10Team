#include "DilationPass.h"
#include "VBAM.h"


DilationPass::DilationPass(void)
{
	ImagePass::filterName = "Dilation filter";
}


DilationPass::~DilationPass(void)
{
}

void DilationPass::processImage(cv::Mat image, cv::Mat &confidence)
{
	//Obtii o alta imagine aplicand unul sau mai multe filtre
	cv::Mat dilatedImage;
	
	cv::dilate(image, dilatedImage, cv::Mat());

	//cv::imwrite("dilated.png", dilatedImage);

	//Compari fiecare pixel si cresti/scazi confidenta dupa ce criterii vrei
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			PixelColor color = getPixelColor(image, i, j);
			PixelColor otherColor = getPixelColor(dilatedImage, i, j);

			float conf = getConfidence(confidence, i, j);

			if(color == otherColor)
				conf *= 1.5;

			setConfidence(confidence, i, j, conf);
		}
	}

}