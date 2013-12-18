#include "VBAM.h"
#include "ErosionPass.h"
#include "DilationPass.h"
#include "NoiseRemovalPass.h"
#include "GradientPass.h"

void initVotePass()
{
	VotePass *votePass = VoteManager::votePass;

	votePass->addPass(new ErosionPass());
	votePass->addPass(new DilationPass());
	votePass->addPass(new GradientPass());
	votePass->addPass(new NoiseRemovalPass());
}


int main(int argc, char** argv)
{	
	initVotePass();

	double t1 = atof(argv[1]);
	double t2 = atof(argv[2]);

	char *bamFolder = argv[3];
	char *bamOutputFolder = argv[5];

	char *inputImage = argv[4];
	char *outputImage = argv[6];

	VBAM::setT1(t1);
	VBAM::setT2(t2);
	VBAM::setBamFolder(bamFolder);
	VBAM::setInputImage(inputImage);
	VBAM::setBamOutputFolder(bamOutputFolder);
	VBAM::setOutputImage(outputImage);

	VBAM::vote();
	
	return 0;
}