#pragma once

#include "VoteManager.h"
#include <string>
#include <sstream>
#include <vector>
#include <fstream>
#include <windows.h>
#include "dirent.h"


#define PREFIX_BINARY_IMAGE "bin_"
#define PREFIX_CONFIDENCE "conf_"

class VBAM
{
public:
	VBAM(void);
	~VBAM(void);

	static void vote(std::string inputFile, std::string outputFile);

private:
	static int runBamPrograms(std::string inputFile);
	static cv::Mat convertImage(cv::Mat image);
	static void cleanup(int count);

private:
	static std::vector<std::string> split(const std::string &s, char delim);

};

