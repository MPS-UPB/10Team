#pragma once

#include "ImagePass.h"

class VotePass
{
public:
	VotePass(void);
	~VotePass(void);

	void addPass(ImagePass *pass);
	void run(cv::Mat &image, cv::Mat &confidence, std::string imageName);

private:
	std::vector<ImagePass*> passes;
};

