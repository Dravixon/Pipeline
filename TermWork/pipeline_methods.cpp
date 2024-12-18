#include "pipeline.h"

Pipeline::Pipeline(float P1, float P2, int N, int M, int commandCount) : size(commandCount)
{
	List = new Command[size];
	for (int i = 0; i < commandCount; i++)
	{
		srand(rand());
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
			while ((canAccess < List[i].readWriteTime) && (tmpShift < op2MemAccess.size()))
			{
				if ((op2MemAccess[tmpShift] + writeMemAccess[tmpShift]) == 2 || op2RegAccess[tmpShift] != 0)
				{
					canAccess = 0;
				}
				else
				{
					canAccess++;
				}
				tmpShift++;
			}
			if (tmpShift >= op2MemAccess.size())
			{
				op2MemAccess.resize(op2MemAccess.size() + (tmpShift - shift + 1) + List[i].readWriteTime + List[i].calculatingTime);
				op2RegAccess.resize(op2MemAccess.size());
				calculateAccess.resize(op2MemAccess.size());
				writeMemAccess.resize(op2MemAccess.size());
				writeRegAccess.resize(op2MemAccess.size());
			}
			for (int j = tmpShift - canAccess; j < tmpShift; j++)
			{
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
				op2RegAccess.resize(tmpShift + 1);
				op2MemAccess.resize(op2RegAccess.size());
				calculateAccess.resize(op2RegAccess.size());
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
			op2RegAccess.resize(calculateAccess.size());
			op2MemAccess.resize(calculateAccess.size());
			writeMemAccess.resize(calculateAccess.size());
			writeRegAccess.resize(calculateAccess.size());
		}
		for (int j = tmpShift - canAccess; j < tmpShift - canAccess + List[i].calculatingTime; j++)
		{
			calculateAccess[j]++;
		}
		List[i].preCalculatingShift = (tmpShift - canAccess - shift);

		shift = tmpShift - canAccess + List[i].calculatingTime;	
		if (shift >= writeMemAccess.size())
		{
			writeMemAccess.resize(shift + 1);
			op2MemAccess.resize(writeMemAccess.size());
			op2RegAccess.resize(writeMemAccess.size());
			calculateAccess.resize(writeMemAccess.size());
			writeRegAccess.resize(writeMemAccess.size());
		}
		tmpShift = shift;

		//preWritingShift calculation
		if(List[i].op2Mem)
		{
			canAccess = 0;	
			while ((canAccess < List[i].readWriteTime) && (tmpShift < writeMemAccess.size()))
			{
				if ((op2MemAccess[tmpShift] + writeMemAccess[tmpShift]) == 2 || writeRegAccess[tmpShift] != 0)
				{
					canAccess = 0;
				}
				else
				{
					canAccess++;
				}
				tmpShift++;
			}
			if ((shift + List[i].readWriteTime) >= writeMemAccess.size())
			{			
				writeMemAccess.resize(writeMemAccess.size() + (tmpShift - shift + 1) + List[i].readWriteTime);
				op2MemAccess.resize(writeMemAccess.size());
				op2RegAccess.resize(writeMemAccess.size());
				calculateAccess.resize(writeMemAccess.size());
				writeRegAccess.resize(writeMemAccess.size());
			}

			for (int j = shift; j < shift + List[i].readWriteTime; j++)
			{
				writeMemAccess[j]++;
			}
			List[i].preWritingShift += (tmpShift - canAccess - shift);
		}
		else
		{
			while (tmpShift < writeRegAccess.size() && (writeRegAccess[tmpShift] != 0 || writeMemAccess[tmpShift] != 0))
			{
				tmpShift++;
			}
			if (tmpShift >= writeRegAccess.size())
			{	
				writeRegAccess.resize(tmpShift + 1);
				op2MemAccess.resize(writeRegAccess.size());
				op2RegAccess.resize(writeRegAccess.size());
				calculateAccess.resize(writeRegAccess.size());
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