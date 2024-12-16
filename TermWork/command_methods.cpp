#include "command.h"
#include <ctime>

Command::Command(double P1, double P2, int N, int M)
{
	srand(std::time(NULL));
	this->op2Mem = ((rand() % 100) < (P1 * 100)) ? false : true;
	this->type = ((rand() % 100) < (P2 * 100)) ? 1 : 2;
	this->readWriteTime = (op2Mem) ? N : 1;
	this->calculatingTime = (type == 2) ? M : 1;

	this->time += readWriteTime * 2 + calculatingTime;
}

void Command::printCommand()
{
	int i;
	for (i = 0; i < initialShift; i++)
		cout << "   ";
	cout << "DC 1R ";
	for (i = 0; i < preOp2Shift; i++)
		cout << "__ ";
	if (op2Mem)
		for (i = 0; i < readWriteTime; i++)
			cout << "2M ";
	else
		cout << "2R ";
	for (i = 0; i < preCalculatingShift; i++)
		cout << "__ ";
	for (i = 0; i < calculatingTime; i++)
		cout << "CR ";
	for (i = 0; i < preWritingShift; i++)
		cout << "__ ";
	if (op2Mem)
		for (i = 0; i < readWriteTime; i++)
			cout << "WM ";
	else
		cout << "WR ";
	cout << endl;
}

int Command::getTime()
{
	return this->time;
}
