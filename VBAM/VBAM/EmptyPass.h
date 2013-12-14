#pragma once
#include "imagepass.h"
class EmptyPass :
	public ImagePass
{
public:
	EmptyPass(void);
	~EmptyPass(void);

	//Copiaza declaratia functiei asteia in headerul claselor ImagePass facute de tine (obligatoriu)
	void processImage(cv::Mat image, cv::Mat &confidence);
};

