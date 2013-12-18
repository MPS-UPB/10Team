#pragma once
#include "imagepass.h"
class NoiseRemovalPass :
	public ImagePass
{
public:
	NoiseRemovalPass(void);
	~NoiseRemovalPass(void);

	void processImage(cv::Mat image, cv::Mat &confidence);
};

