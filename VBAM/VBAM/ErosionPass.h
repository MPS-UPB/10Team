#pragma once
#include "imagepass.h"

class ErosionPass:
	public ImagePass
{
public:
	ErosionPass(void);
	~ErosionPass(void);

	void processImage(cv::Mat image, cv::Mat &confidence);
};

