
#include "VBAM.h"


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

int VBAM::runBamPrograms(std::string inputFile)
{
	//TODO - remove in final implementation
	LPCWSTR lpPathName = L"test_program";
	if(!SetCurrentDirectory(lpPathName))
	{
		printf("SetCurrentDirectory failed (%d)\n", GetLastError());
	}

	FILE *file = fopen(inputFile.c_str(), "r");
	if (!file) 
	{
		printf("Error: Input file not found\n");
		std::cin.get();
		std::exit(1);
	}

	fclose(file);

	int count = 0;

	char buffer[MAX_PATH];
	GetModuleFileNameA(NULL, buffer, MAX_PATH);

	std::vector<std::string> pathElems = split(buffer, '\\');
	std::string currentFile = pathElems[pathElems.size() - 1];


	printf("Running BAM programs\n");
	printf("-------------------------------\n");

	std::string folder = ".";
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
				std::string extention = elems[elems.size() - 1];

				if(extention.compare("exe") == 0)
				{
					memset(command, 0, 100);
					sprintf_s(command, "%s %s %s%d %s%d", file.c_str(), inputFile.c_str(), PREFIX_BINARY_IMAGE, count, PREFIX_CONFIDENCE, count);
					printf("Running %s...\n", file.c_str());

					std::system(command);

					count++;
				}

				else
				{
				}
			}
		}
		closedir (dir);

		printf("\n");
	} 
	else 
	{
		perror ("");
		return 0;
	}

	return count;
}

cv::Mat VBAM::convertImage(cv::Mat image)
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

void VBAM::vote(std::string inputFile, std::string outputFile)
{
	printf("Starting VBAM\n\n");

	printf("Input filename: %s\n", inputFile.c_str());
	printf("Output filename: %s\n", outputFile.c_str());
	
	printf("\n");

	int imageCount = runBamPrograms(inputFile);
	
	if(imageCount == 0)
	{
		printf("Error: No BAM programs found\n");
		std::cin.get();
		std::exit(1);
	}

	cv::Mat originalImage = cv::imread(inputFile);
	VoteManager::init(originalImage.rows, originalImage.cols);

	for(int i=0; i<imageCount; i++)
	{
		char binaryName[50];
		char confidenceName[50];

		sprintf(binaryName, "%s%d", PREFIX_BINARY_IMAGE, i);
		sprintf(confidenceName, "%s%d", PREFIX_CONFIDENCE, i);

		cv::Mat image = convertImage(cv::imread(binaryName));
		cv::Mat confidence = convertImage(cv::imread(confidenceName));

		VoteManager::addInput(image, confidence);
	}

	cv::Mat result = VoteManager::run();

	cleanup(imageCount);

	printf("Writing final image...\n");

	cv::imwrite(outputFile, result);
	cv::namedWindow("Test result", cv::WINDOW_NORMAL);
	cv::imshow("Test result",result);

	printf("Finished!\n");
	cvWaitKey(0);
}
