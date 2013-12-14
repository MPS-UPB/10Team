#include "VoteManager.h"

VotePass *VoteManager::votePass = new VotePass();

int VoteManager::imageCount;

int VoteManager::imageWidth;
int VoteManager::imageHeight;

std::vector<cv::Mat> VoteManager::images;
std::vector<cv::Mat> VoteManager::confidences;

cv::Mat VoteManager::blackConfidence;
cv::Mat VoteManager::whiteConfidence;

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

	VoteManager::blackConfidence = cv::Mat(imageWidth, imageHeight, cv::DataType<uchar>::type, cvScalar(0));
	VoteManager::whiteConfidence = cv::Mat(imageWidth, imageHeight, cv::DataType<uchar>::type, cvScalar(0));

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

cv::Mat VoteManager::run()
{
	cv::Mat finalImage = cv::Mat(imageWidth, imageHeight, cv::DataType<uchar>::type, cvScalar(0));

	printf("Applying filters to binary images\n");
	printf("-------------------------------\n");

	for(int i=0; i<VoteManager::imageCount; i++)
	{
		printf("Processing image %d...\n", i);

		cv::Mat image = VoteManager::images[i];
		cv::Mat confidence = VoteManager::confidences[i];

		VoteManager::votePass->run(image, confidence);

		for(int j=0; j<confidence.rows; j++)
		{
			for(int k=0; k<confidence.cols; k++)
			{
				uchar pixelValue = image.at<uchar>(j, k);
				uchar pixelConfidence = confidence.at<uchar>(j, k);

				if(pixelValue == 0)
				{
					ImagePass::addValue(VoteManager::blackConfidence.at<uchar>(j, k), pixelConfidence);
				}
				else
				{
					ImagePass::addValue(VoteManager::whiteConfidence.at<uchar>(j, k), pixelConfidence);
				}
			}
		}

		printf("\n");
	}

	printf("Voting...\n");

	for(int i=0; i<VoteManager::imageWidth; i++)
	{
		for(int j=0; j<VoteManager::imageHeight; j++)
		{
			uchar pixelBlackConfidence = VoteManager::blackConfidence.at<uchar>(i, j);
			uchar pixelWhiteConfidence = VoteManager::whiteConfidence.at<uchar>(i, j);

			if(pixelBlackConfidence < pixelWhiteConfidence)
			{
				finalImage.at<uchar>(i, j) = 255;
			}
			else
			{
				finalImage.at<uchar>(i, j) = 0;
			}
		}
	}

	return finalImage;
}