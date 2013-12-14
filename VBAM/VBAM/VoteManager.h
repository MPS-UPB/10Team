#pragma once

#include <opencv\highgui.h>
#include "VotePass.h"

class VoteManager
{
public:
	VoteManager(void);
	~VoteManager(void);

	static void addInput(cv::Mat image, cv::Mat confidence);
	static void setVotePass(VotePass *votePass);

	static void init(int imageWidth, int imageHeight);
	static cv::Mat run();

public:
	static VotePass *votePass;

private:
	static int imageCount;

	static int imageWidth;
	static int imageHeight;

	static std::vector<cv::Mat> images;
	static std::vector<cv::Mat> confidences;
	
	static cv::Mat blackConfidence;
	static cv::Mat whiteConfidence;
};

