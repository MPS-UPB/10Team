#include "VBAM.h"
#include <process.h>
#include <Tlhelp32.h>
#include <winbase.h>
#include <string.h>

std::thread VBAM::killThread;
std::vector<std::thread*> VBAM::bamThreads;

double VBAM::t1;
double VBAM::t2;
std::string VBAM::bamFolder;
std::string VBAM::inputImage;
std::string VBAM::bamOutputFolder;
std::string VBAM::outputImage;

std::vector<std::string> VBAM::bamArray;
std::map<std::string, bool> VBAM::bamDone;

VBAM::VBAM(void)
{
}


VBAM::~VBAM(void)
{
}

std::vector<std::string> VBAM::split(const std::string &s, char delim) 
{
	std::stringstream ss(s);
	std::string item;
	std::vector<std::string> elems;

	while (std::getline(ss, item, delim))
	{
		elems.push_back(item);
	}
	return elems;
}

void VBAM::printBamExitCode(std::string bam, int errorCode)
{
	printf("%s: ", bam.c_str());

	switch (errorCode)
	{
	case -1:
		printf("Error opening input file\n");
		break;

	case -2:
		printf("Error processing input file\n");
		break;

	case -3:
		printf("Error writing output file\n");
		break;

	case -4:
		printf("Out of memory\n");
		break;

	default:
		printf("Finished successfully\n");
		break;
	}
}

void VBAM::startBamProgram(std::string bam, std::string command)
{
	int exitCode = std::system(command.c_str());

	if(exitCode == 0)
	{
		VBAM::bamDone[bam] = true;
	}

	if(exitCode <= 0)
	{
		printBamExitCode(bam, exitCode);
	}
}

void VBAM::startTimer(std::string bam, double time)
{
	char command[100];

	Sleep(time);
	
	if(!bamDone[bam])
	{
		memset(command, 0, 100);
		sprintf(command, "taskkill /f /im %s.exe", bam.c_str());

		std::system(command);
	}
	
}

void VBAM::runBamPrograms(std::string inputFile, double timeOut)
{
	

	FILE *file = fopen(inputFile.c_str(), "r");
	if (!file) 
	{
		printf("Error: Input file not found\n");
		std::cin.get();
		std::exit(1);
	}

	fclose(file);

	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::vector<std::string> pathElems = split(buffer, '\\');
	std::string currentFile = pathElems[pathElems.size() - 1];

	std::vector<std::string> inputFileElems = split(inputFile, '.');
	std::string inputFileName = inputFileElems[0];

	printf("Running BAM programs\n");
	printf("-------------------------------\n");

	std::string folder = VBAM::bamFolder;
	DIR *dir;
	struct dirent *ent;

	if ((dir = opendir (folder.c_str())) != NULL) 
	{
		
		char command[100];

		while ((ent = readdir (dir)) != NULL) 
		{
			std::string file = ent->d_name;

			if(file.compare(currentFile) == 0)
			{
				continue;
			}
				

			std::vector<std::string> elems = split(file, '.');

			if(elems.size() > 1)
			{
				std::string filename = elems[0];
				std::string extention = elems[elems.size() - 1];

				if(extention.compare("exe") == 0)
				{
					memset(command, 0, 100);
					sprintf_s(command, "%s\\%s %s %s\\%s_%s.tif %s\\%s_%s_conf.tif", VBAM::bamFolder.c_str(), file.c_str(), inputFile.c_str(), VBAM::bamOutputFolder.c_str(), filename.c_str(), inputFileName.c_str(), VBAM::bamOutputFolder.c_str(), filename.c_str(), inputFileName.c_str());
					printf("Running %s...\n", filename.c_str());

					

					double startTime = GetTickCount() / 1000.0f;

					killThread = std::thread(startTimer, filename, timeOut);
					startBamProgram(filename, command);

					double endTime = GetTickCount() / 1000.0f;

					printf("Run time: %.3f sec\n\n", endTime - startTime);

					killThread.detach();

					VBAM::bamArray.push_back(filename);
				}
			}
		}
		closedir (dir);

		
	} 
	else 
	{
		return;
	}

	printf("\n");
}

cv::Mat VBAM::convertImage(cv::Mat image, bool toFloat)
{
	cv::Mat result;

	switch (image.channels())
	{
	
	case 1:
		{
			result = image;
			break;
		}
	case 3:
		{
			cv::cvtColor(image, result, CV_BGR2GRAY);
			break;
		}

	case 4:
		{
			cv::cvtColor(image, result, CV_BGRA2GRAY);
			break;
		}
	default:
		break;
	}

	if(toFloat)
		result.convertTo(result, CV_32FC1);

	return result;
}

void VBAM::cleanup(int count)
{
	printf("Cleaning up...\n");

	for(int i=0; i<count; i++)
	{
		char binaryName[50];
		char confidenceName[50];

		sprintf(binaryName, "%s%d", PREFIX_BINARY_IMAGE, i);
		sprintf(confidenceName, "%s%d", PREFIX_CONFIDENCE, i);

		remove(binaryName);
		remove(confidenceName);
	}
}

void VBAM::setT1(double t1)
{
	VBAM::t1 = t1;
}

void VBAM::setT2(double t2)
{
	VBAM::t2 = t2;
}


void VBAM::setBamFolder(std::string bamFolder)
{
	VBAM::bamFolder = bamFolder;
}

void VBAM::setInputImage(std::string inputImage)
{
	VBAM::inputImage = inputImage;
}

void VBAM::setBamOutputFolder(std::string bamOutputFolder)
{
	VBAM::bamOutputFolder = bamOutputFolder;
}

void VBAM::setOutputImage(std::string outputImage)
{
	VBAM::outputImage = outputImage;
}

void VBAM::vote()
{
	//TODO - remove in final implementation
	LPCWSTR lpPathName = L"test_program";
	if(!SetCurrentDirectory(lpPathName))
	{
		printf("SetCurrentDirectory failed (%d)\n", GetLastError());
	}

	cv::Mat originalImage = cv::imread(VBAM::inputImage);
	double timeOut = VBAM::t1 * originalImage.rows * originalImage.cols + VBAM::t2;

	printf("Starting VBAM\n\n");

	printf("T1: %.3f ms\n", VBAM::t1);
	printf("T2: %.3f ms\n", VBAM::t2);
	printf("Total timeout: %.3f sec\n\n", timeOut / 1000.0f);

	printf("BAM folder: %s\n", VBAM::bamFolder.c_str());
	printf("BAM output folder: %s\n\n", VBAM::bamOutputFolder.c_str());

	printf("Input filename: %s\n", VBAM::inputImage.c_str());
	printf("Output filename: %s\n\n", VBAM::outputImage.c_str());
	

	runBamPrograms(VBAM::inputImage, timeOut);
	
	
	VoteManager::init(originalImage.rows, originalImage.cols);

	std::vector<std::string> inputFileElems = split(VBAM::inputImage, '.');
	std::string inputFileName = inputFileElems[0];

	int count = 0;

	for(std::string bamProgram : bamArray)
	{
		if(!bamDone[bamProgram])
			continue;

		char binaryName[50];
		char confidenceName[50];

		sprintf(binaryName, "%s\\%s_%s.tif", VBAM::bamOutputFolder.c_str(), bamProgram.c_str(),inputFileName.c_str());
		sprintf(confidenceName, "%s\\%s_%s_conf.tif", VBAM::bamOutputFolder.c_str(), bamProgram.c_str(), inputFileName.c_str());

		cv::Mat image = convertImage(cv::imread(binaryName), false);
		cv::Mat confidence = convertImage(cv::imread(confidenceName), true);

		VoteManager::addInput(image, confidence);

		count++;
	}

	if(count == 0)
	{
		printf("Error: All BAM programs were killed\n");

		std::cin.get();

		std::exit(1);
	}

	cv::Mat result = VoteManager::run();

	printf("Writing final image...\n");

	cv::imwrite(VBAM::outputImage, result);
	cv::namedWindow("Test result", cv::WINDOW_NORMAL);
	cv::imshow("Test result",result);

	printf("Finished!\n");
	cvWaitKey(0);
}
