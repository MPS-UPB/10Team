#include "VBAM.h"
#include "ErosionPass.h"
#include "DilationPass.h"
#include "NoiseRemovalPass.h"
#include "GradientPass.h"

void initVotePass()
{
	VotePass *votePass = VoteManager::votePass;

	//votePass->addPass(new NoiseRemovalPass());
	votePass->addPass(new ErosionPass());
	//votePass->addPass(new DilationPass());
	//votePass->addPass(new GradientPass());

	/*
	Noutati:
		Am sters metoda "addValue()" din ImagePass (adaugati confidentele normal acum)
		Confidentele sunt acum de tip float
		IMPORTANT!
			Dupa ce ati obtinut o confidenta folosind "getConfidence()", pentru a o actualiza aveti urmatoarele optiuni:
				- declarati o variabila in felul urmator
					float &confidence = getConfidence(confidence, i, j);
					confidence += someConfidenceValue;

				- daca vi se pare enervant, aveti o a doua optiune
					float confidence = getConfidence(confidence, i, j);
					confidence += someConfidenceValue;
					setConfidence(confidence, i, j, confidence);

				- VARIANTA URMATOARE NU FUNCTIONEAZA!
					float confidence = getConfidence(confidence, i, j);
					confidence += someConfidenceValue;

		Visualizarea imaginilor editate nu mai functioneaza in cadrul unui ImagePass. Salvati-le in fisier daca vreti sa faceti debugging pe ele:
			cv::imwrite(file_name, image);

		Functia "fastNlMeansDenoising" nu mai da eroare de link-are, dar se pare ca nu merge (nu se termina). O sa ma mai uit pe ea.


	*/
}


int main(int argc, char** argv)
{	
	//Parametri:  imagine_intrare imagine_iesire
	//Pentru modificarea parametrilor de rulare:
	//Click dreapta pe proiect in "Solution Explorer" (se cheama VBAM)
	//Properties
	//Debugging
	//Le scrii in dreapta la "Command Arguments"

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