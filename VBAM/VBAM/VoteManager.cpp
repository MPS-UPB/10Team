#include "VoteManager.h"
#include <windows.h>

#define infinity 100000

VotePass *VoteManager::votePass = new VotePass();

int VoteManager::imageCount;

int VoteManager::imageWidth;
int VoteManager::imageHeight;

std::vector<cv::Mat> VoteManager::images;
std::vector<cv::Mat> VoteManager::confidences;

cv::Mat VoteManager::blackConfidence;
cv::Mat VoteManager::whiteConfidence;

std::vector<std::thread*> VoteManager::processThreads;
std::mutex VoteManager::mtx;

VoteManager::VoteManager(void)
{
	
}


VoteManager::~VoteManager(void)
{
}

void VoteManager::init(int imageWidth, int imageHeight)
{
	VoteManager::imageWidth = imageWidth;
	VoteManager::imageHeight = imageHeight;

	VoteManager::blackConfidence = cv::Mat(imageWidth, imageHeight, cv::DataType<float>::type, cvScalar(0));
	VoteManager::whiteConfidence = cv::Mat(imageWidth, imageHeight, cv::DataType<float>::type, cvScalar(0));

	VoteManager::imageCount = 0;
}

void VoteManager::addInput(cv::Mat image, cv::Mat confidence)
{
	VoteManager::images.push_back(image);
	VoteManager::confidences.push_back(confidence);

	VoteManager::imageCount++;
}

void VoteManager::setVotePass(VotePass *votePass)
{
	VoteManager::votePass = votePass;
}

void VoteManager::processImage(int index)
{
	//mtx.lock();

	cv::Mat image = VoteManager::images[index];
	cv::Mat confidence = VoteManager::confidences[index];

	VoteManager::votePass->run(image, confidence);


	for(int j=0; j<confidence.rows; j++)
	{
		for(int k=0; k<confidence.cols; k++)
		{
			PixelColor pixelValue = ImagePass::getPixelColor(image, j, k);
			float pixelConfidence = confidence.at<float>(j, k);

			if(pixelValue == PIXEL_BLACK)
			{
				VoteManager::blackConfidence.at<float>(j, k) += pixelConfidence;
			}
			else
			{
				VoteManager::whiteConfidence.at<float>(j, k) += pixelConfidence;
			}
		}
	}

	//mtx.unlock();
}

cv::Mat VoteManager::run()
{
	cv::Mat finalImage = cv::Mat(imageWidth, imageHeight, cv::DataType<uchar>::type, cvScalar(0));

	printf("Applying filters to binary images\n");
	printf("-------------------------------\n");

	for(int i=0; i<VoteManager::imageCount; i++)
	{
		std::thread *newThread = new std::thread(processImage, i);

		VoteManager::processThreads.push_back(newThread);
	}

	for(std::thread *processThread : VoteManager::processThreads)
	{
		processThread->join();
	}

	printf("Done!\n\n");

	printf("Voting...\n");

	for(int i=0; i<VoteManager::imageWidth; i++)
	{
		for(int j=0; j<VoteManager::imageHeight; j++)
		{
			float pixelBlackConfidence = VoteManager::blackConfidence.at<float>(i, j);
			float pixelWhiteConfidence = VoteManager::whiteConfidence.at<float>(i, j);

			if ( (pixelBlackConfidence < pixelWhiteConfidence && pixelWhiteConfidence != 0) 
				|| ( pixelBlackConfidence == pixelWhiteConfidence ) ) 
			{
				finalImage.at<uchar>(i, j) = 255;
			}
			else if ( pixelWhiteConfidence < pixelBlackConfidence && pixelBlackConfidence != 0 )
			{
				finalImage.at<uchar>(i, j) = 0;
			}
		}
	}

	return finalImage;
}