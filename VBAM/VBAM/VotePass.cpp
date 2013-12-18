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

void VotePass::run(cv::Mat &image, cv::Mat &confidence, std::string imageName)
{
	for(int i=0; i<passes.size(); i++)
	{
		ImagePass *pass = passes[i];

		printf("\tApplying %s for %s\n", pass->filterName.c_str(), imageName.c_str());

		pass->processImage(image, confidence);

	}

}
