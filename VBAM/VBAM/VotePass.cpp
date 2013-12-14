#include "VotePass.h"


VotePass::VotePass(void)
{
}


VotePass::~VotePass(void)
{
}

void VotePass::addPass(ImagePass *pass)
{
	this->passes.push_back(pass);
}

void VotePass::run(cv::Mat &image, cv::Mat &confidence)
{

	
	for(int i=0; i<passes.size(); i++)
	{
		ImagePass *pass = passes[i];

		printf("\tApplying filter %d\n", i);

		pass->processImage(image, confidence);

	}

}
