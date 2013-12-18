#pragma once

#include "VoteManager.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <thread>
#include <windows.h>
#include <map>
#include "dirent.h"


#define PREFIX_BINARY_IMAGE "bin_"
#define PREFIX_CONFIDENCE "conf_"

class VBAM
{
public:
	VBAM(void);
	~VBAM(void);

	static void setT1(double t1);
	static void setT2(double t2);
	static void setBamFolder(std::string bamFolder);
	static void setInputImage(std::string inputImage);
	static void setBamOutputFolder(std::string bamOutputFolder);
	static void setOutputImage(std::string outputImage);

	static void vote();

private:
	static void runBamPrograms(std::string inputFile, double timeOut);
	static cv::Mat convertImage(cv::Mat image, bool toFloat);
	static void cleanup(int count);

private:
	static std::vector<std::string> split(const std::string &s, char delim);
	static void startBamProgram(std::string bam, std::string command);
	static void startTimer(std::string bam, double time);
	static void printBamExitCode(std::string bam, int errorCode);

private:
	static std::thread killThread;
	static std::vector<std::thread*> bamThreads;

	static double t1;
	static double t2;
	static std::string bamFolder;
	static std::string inputImage;
	static std::string bamOutputFolder;
	static std::string outputImage;

	static std::vector<std::string> bamArray;
	static std::map<std::string, bool> bamDone;
};

