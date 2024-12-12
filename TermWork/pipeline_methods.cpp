#include "pipeline.h"

Pipeline::Pipeline(float P1, float P2, int N, int M, int commandCount) : size(commandCount)
{
	List = new Command[size];
	for (int i = 0; i < commandCount; i++)
	{
		Command C(P1, P2, N, M);
		C.initialShift = i;
		List[i] = C;
	}
}

Pipeline::~Pipeline()
{
	if (this->List != NULL)
		delete[] List;
}

void Pipeline::printPipeline()
{
	for (int i = 0; i < size; i++)
		List[i].printCommand();
}

int Pipeline::getLongestCommandTime()
{
	int maxTime = 0;
	for (int i = 0; i < size; i++)
	{
		if ((List[i].getTime() + List[i].initialShift) > maxTime)
			maxTime = (List[i].getTime() + List[i].initialShift);
	}
	return maxTime;
}

void Pipeline::SetShifts()
{
	vector<int> memoryAccess(this->getLongestCommandTime(), 0);
	vector<int> op2MemAccess(this->getLongestCommandTime(), 0);
	vector<int> op2RegAccess(this->getLongestCommandTime(), 0);
	vector<int> calculateAccess(this->getLongestCommandTime(), 0);
	vector<int> writeMemAccess(this->getLongestCommandTime(), 0);
	vector<int> writeRegAccess(this->getLongestCommandTime(), 0);

	for (int i = 0; i < size; i++)
	{
		int shift = List[i].initialShift + 2;
		int canAccess = 0;
		int tmpShift = shift;

		//preOp2Shift calculation
		if (List[i].op2Mem)
		{
			while ((canAccess < List[i].readWriteTime) && (tmpShift < memoryAccess.size()))
			{
				if (memoryAccess[tmpShift] == 2 || op2RegAccess[tmpShift] != 0)
				{
					canAccess = 0;
				}
				else
				{
					canAccess++;
				}
				tmpShift++;
			}
			if (tmpShift >= memoryAccess.size())
			{
				memoryAccess.resize(memoryAccess.size() + (tmpShift - shift + 1) + List[i].readWriteTime + List[i].calculatingTime);
				op2MemAccess.resize(memoryAccess.size());
				op2RegAccess.resize(memoryAccess.size());
				writeMemAccess.resize(memoryAccess.size());
				writeRegAccess.resize(memoryAccess.size());
			}
			for (int j = tmpShift - canAccess; j < tmpShift; j++)
			{
				memoryAccess[j]++;
				if (op2MemAccess[j] < 1)
					op2MemAccess[j]++;
			}
			List[i].preOp2Shift += (tmpShift - canAccess - shift);		
		}
		else
		{
			while ((op2RegAccess[tmpShift] != 0 || op2MemAccess[tmpShift] != 0) && tmpShift < op2RegAccess.size())
				tmpShift++;
			if (tmpShift >= op2RegAccess.size())
			{
				op2RegAccess.resize(tmpShift);
				memoryAccess.resize(op2RegAccess.size());
				op2MemAccess.resize(op2RegAccess.size());		
				writeMemAccess.resize(op2RegAccess.size());
				writeRegAccess.resize(op2RegAccess.size());
			}
			op2RegAccess[tmpShift]++;
			List[i].preOp2Shift += (tmpShift - shift);
		}
		shift += List[i].preOp2Shift + List[i].readWriteTime;

		canAccess = 0;
		tmpShift = shift;

		//preCalculatingShift calculation
		while ((canAccess < List[i].calculatingTime) && (tmpShift < calculateAccess.size()))
		{
			if (calculateAccess[tmpShift] == 1)
			{
				canAccess = 0;
			}
			else
			{
				canAccess++;
			}
			tmpShift++;
		}
		if (tmpShift - canAccess + List[i].calculatingTime >= calculateAccess.size())
		{
			calculateAccess.resize(tmpShift - canAccess + List[i].calculatingTime);
		}
		for (int j = tmpShift - canAccess; j < tmpShift - canAccess + List[i].calculatingTime; j++)
		{
			calculateAccess[j]++;
		}
		List[i].preCalculatingShift = (tmpShift - canAccess - shift);

		shift = tmpShift - canAccess + List[i].calculatingTime;	
		if (shift >= memoryAccess.size())
		{
			memoryAccess.resize(shift);
			op2MemAccess.resize(memoryAccess.size());
			op2RegAccess.resize(memoryAccess.size());
			writeMemAccess.resize(memoryAccess.size());
			writeRegAccess.resize(memoryAccess.size());
		}

		//preWritingShift calculation
		if(List[i].op2Mem)
		{
			canAccess = 0;
			tmpShift = shift;
			while ((canAccess < List[i].readWriteTime) && (tmpShift < memoryAccess.size()))
			{
				if (memoryAccess[tmpShift] == 2 || writeRegAccess[tmpShift] != 0)
				{
					canAccess = 0;
				}
				else
				{
					canAccess++;
				}
				tmpShift++;
			}
			if ((shift + List[i].readWriteTime) >= memoryAccess.size())
			{			
				memoryAccess.resize(memoryAccess.size() + (tmpShift - shift + 1) + List[i].readWriteTime);
				op2MemAccess.resize(memoryAccess.size());
				op2RegAccess.resize(memoryAccess.size());
				writeMemAccess.resize(memoryAccess.size());
				writeRegAccess.resize(memoryAccess.size());
			}

			for (int j = shift; j < shift + List[i].readWriteTime; j++)
			{
				memoryAccess[j]++;
				if (writeMemAccess[j] < 1)
					writeMemAccess[j]++;
			}
			List[i].preWritingShift += (tmpShift - canAccess - shift);
		}
		else
		{
			while ((writeRegAccess[tmpShift] != 0 || writeMemAccess[tmpShift]) && tmpShift < writeRegAccess.size())
				tmpShift++;
			if (tmpShift >= writeRegAccess.size())
			{	
				writeRegAccess.resize(tmpShift);
				memoryAccess.resize(writeRegAccess.size());
				op2MemAccess.resize(writeRegAccess.size());
				op2RegAccess.resize(writeRegAccess.size());
				writeMemAccess.resize(writeRegAccess.size());
			}
			writeRegAccess[tmpShift]++;
			List[i].preWritingShift += (tmpShift - shift);
		}
		
		List[i].time += List[i].preOp2Shift + List[i].preCalculatingShift + List[i].preWritingShift;
	}
}

double Pipeline::getAverageTime()
{
	double sum = 0;
	for (int i = 0; i < this->size; i++)
		sum += List[i].getTime();
	return sum / this->size;
}