#pragma once
#include "header.h"

class Command
{
	//default paramethers
	int type;
	bool op2Mem;
	int readWriteTime;
	int calculatingTime;
	//time
	int time = 2; //2 because decyphering and accessing operand#1 always take 1 tick each
	//pipeline time shifts
	int initialShift = 0;
	int preOp2Shift = 0;
	int preCalculatingShift = 0;
	int preWritingShift = 0;
public:
	Command() : type(0), op2Mem(false), readWriteTime(0), calculatingTime(0) {};
	Command(double P1, double P2, int N, int M);
	void printCommand();
	int getTime();

	friend class Pipeline;
};
