#include "header.h"
#include "command.h"
#include "pipeline.h"

int main()
{
	double P1, P2;
	int N, M;
	int num = 0, button = 0;

	cout << "Choose the probability that the second operand is stored in the register (P1):" << endl
		<< "Press '1': P1 = 0.9" << endl
		<< "Press '2': P1 = 0.8" << endl
		<< "Press '3': P1 = 0.6" << endl;
	while (button != 1 && button != 2 && button != 3)
	{
		cin >> button;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input!" << endl;
		}
		else if (button != 1 && button != 2 && button != 3)
			cout << "Invalid input!" << endl;
	}
	switch (button)
	{
		case(1):
		{
			P1 = 0.9;
			break;
		}
		case(2):
		{
			P1 = 0.8;
			break;
		}
		case(3):
		{
			P1 = 0.6;
			break;
		}
	}
	cout << "Choose the number of ticks it takes to access the second operand (N):" << endl
		<< "Press '1': N = 2" << endl
		<< "Press '2': N = 5" << endl
		<< "Press '3': N = 10" << endl;
	button = 0;
	while (button != 1 && button != 2 && button != 3)
	{
		cin >> button;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input!" << endl;
		}
		else if (button != 1 && button != 2 && button != 3)
			cout << "Invalid input!" << endl;
	}
	switch (button)
	{
		case(1):
		{
			N = 2;
			break;
		}
		case(2):
		{
			N = 5;
			break;
		}
		case(3):
		{
			N = 10;
			break;
		}
	}
	cout << "Choose the number of ticks it takes to calculate the result of a command of a second type (M):" << endl
		<< "Press '1': M = 4" << endl
		<< "Press '2': M = 8" << endl
		<< "Press '3': M = 16" << endl;
	button = 0;
	while (button != 1 && button != 2 && button != 3)
	{
		cin >> button;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input!" << endl;
		}
		else if (button != 1 && button != 2 && button != 3)
			cout << "Invalid input!" << endl;
	}
	switch (button)
	{
		case(1):
		{
			M = 4;
			break;
		}
		case(2):
		{
			M = 8;
			break;
		}
		case(3):
		{
			M = 16;
			break;
		}
	}
	cout << "Choose the probability that the command's type is the first one (P2):" << endl
		<< "Press '1': P2 = 0.9" << endl
		<< "Press '2': P2 = 0.7" << endl
		<< "Press '3': P2 = 0.5" << endl;
	button = 0;
	while (button != 1 && button != 2 && button != 3)
	{
		cin >> button;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input!" << endl;
		}
		else if (button != 1 && button != 2 && button != 3)
			cout << "Invalid input!" << endl;
	}
	switch (button)
	{
		case(1):
		{
			P2 = 0.9;
			break;
		}
		case(2):
		{
			P2 = 0.7;
			break;
		}
		case(3):
		{
			P2 = 0.5;
			break;
		}
	}
	cout << "Input the number of commands: ";
	while (num <= 0)
	{
		cin >> num;
		if (cin.fail())
		{
			cin.clear();
			cin.ignore(numeric_limits<streamsize>::max(), '\n');
			cout << "Invalid input!" << endl;
		}
		else if (num <= 0)
			cout << "Invalid input!" << endl;
	}

	cout << "Current configuration: " << endl
		<< "P1 = " << P1 << endl
		<< "N = " << N << endl
		<< "M = " << M << endl
		<< "P2 = " << P2 << endl
		<< "Number of commands: " << num << endl;
 	cout << "The pipeline:" << endl;

	Pipeline P(P1, P2, N, M, num);
	P.SetShifts();
	P.printPipeline();
	cout << endl << "Average command execution time: " << P.getAverageTime() << " ticks." << endl;
	
	return 0;
}