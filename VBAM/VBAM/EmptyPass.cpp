#include "EmptyPass.h"


EmptyPass::EmptyPass(void)
{
}


EmptyPass::~EmptyPass(void)
{
}


//Exemplu de implementare - nu face absolut nimic :)
void EmptyPass::processImage(cv::Mat image, cv::Mat &confidence)
{
	//Obtii o alta imagine aplicand unul sau mai multe filtre
	cv::Mat otherImage = image;


	//Compari fiecare pixel si cresti/scazi confidenta dupa ce criterii vrei
	for(int i=0; i<image.rows; i++)
	{
		for(int j=0; j<image.cols; j++)
		{
			PixelColor color = getPixelColor(image, i, j);
			PixelColor otherColor = getPixelColor(otherImage, i, j);

			//Confidenta e neaparat de tip uchar
			uchar conf = getConfidence(confidence, i, j);

			if(color == otherColor)
				continue;

			//Pentru modificare confidentei foloseste obligatoriu functia "addValue"
			if(color == PIXEL_BLACK && otherColor == PIXEL_WHITE)
			{
				addValue(conf, 10);
			}
			
			if(color == PIXEL_WHITE && otherColor == PIXEL_BLACK)
			{
				addValue(conf, -10);
			}
			
		}
	}
}
