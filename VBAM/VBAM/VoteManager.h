#pragma once


#include <opencv\highgui.h>
#include <thread>
#include <mutex>
#include "VotePass.h"

class VoteManager
{
public:
	VoteManager(void);
	~VoteManager(void);

	static void addInput(cv::Mat image, cv::Mat confidence, std::string imageName);
	static void setVotePass(VotePass *votePass);

	static void init(int imageWidth, int imageHeight);
	static cv::Mat run();

private:
	static void processImage(int index);

public:
	static VotePass *votePass;

private:
	static int imageCount;

	static int imageWidth;
	static int imageHeight;

	static std::vector<cv::Mat> images;
	static std::vector<cv::Mat> confidences;
	static std::vector<std::string> imageNames;

	static cv::Mat blackConfidence;
	static cv::Mat whiteConfidence;

	static cv::Mat blackModifications;
	static cv::Mat whiteModifications;

	static std::vector<std::thread*> processThreads;
};

