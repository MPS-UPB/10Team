#pragma once
#include "imagepass.h"
class DilationPass :
	public ImagePass
{
public:
	DilationPass(void);
	~DilationPass(void);

	void processImage(cv::Mat image, cv::Mat &confidence);
};

