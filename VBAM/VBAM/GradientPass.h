#pragma once
#include "ImagePass.h"
class GradientPass : 
	public ImagePass
{
public:
	GradientPass(void);
	~GradientPass(void);

	void processImage(cv::Mat image, cv::Mat &confidence);
};

