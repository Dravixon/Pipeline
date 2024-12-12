#pragma once
#include "header.h"
#include "command.h"

class Pipeline
{
	int size;
	Command* List;
public:
	Pipeline(float P1, float P2, int N, int M, int commandCount);
	~Pipeline();
	void printPipeline();
	int getLongestCommandTime();
	void SetShifts();
	double getAverageTime();
};