#include "VBAM.h"
#include "EmptyPass.h"


void initVotePass()
{
	VotePass *votePass = VoteManager::votePass;

	votePass->addPass(new EmptyPass());

	/*
	TODO

	Sterge linia: "votePass->addPass(new EmptyPass());"
	Adauga pass-urile asa:

	votePass->addPass(new MyFirstPass());
	votePass->addPass(new MySecondPass());
	...

	Vezi "EmptyPass.h" si "EmptyPass.cpp" pentru un exemplu basic de implementare

	*/
}


int main(int argc, char** argv)
{
	initVotePass();
	
	//Parametri:  imagine_intrare imagine_iesire
	//Pentru modificarea parametrilor de rulare:
	//Click dreapta pe proiect in "Solution Explorer" (se cheama VBAM)
	//Properties
	//Debugging
	//Le scrii in dreapta la "Command Arguments"

	VBAM::vote(argv[1], argv[2]);
	
	return 0;
}