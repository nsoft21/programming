#include "pch.h"
#include <iostream>
using namespace std;

double correctness()
{
	while (true)
	{
		double a;
		cin >> a;

		if (cin.fail())
		{
			cin.clear();
			cin.ignore(32767, '\n');
			cout << "Неверный формат ввода. Введите число.\n";
		}
		else
		{
			return a;
		}
	}
}

char getOperator()
{
	while (true) 
	{
		char sm;
		cin >> sm;
		if (sm == '+' || sm == '-' || sm == '*' || sm == '/') {
			return sm;
		}
		else {
			cin.clear();
			cin.ignore(32767, '\n');
			std::cout << "Неверный формат ввода. Выберите \"+\", \"-\" \"*\" или \"/\".\n";
		}
	}
}

void printResult(double a, char sm, double b)
{
	switch (sm) {
	case '+':
		cout << a + b;
		break;
	case '-':
		cout << a - b;
		break;
	case '*':
		cout << a * b;
		break;
	case '/':
		cout << a / b;
		break;
	default:
		cout << "Неверный формат ввода.";
		break;
	}
}

int main()
{
	setlocale(LC_ALL, "rus");
	double a = correctness();
	char sm = getOperator();
	double b = correctness();
	printResult(a, sm, b);
	return 0;
}