#include <iostream>

double correctness()
{
	while (true)
	{
		double a;
		std::cin >> a;

		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Неверный формат ввода. Введите число.\n";
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
		std::cin >> sm;
		if (sm == '+' || sm == '-' || sm == '*' || sm == '/') {
			return sm;
		}
		else {
			std::cin.clear();
			std::cin.ignore(32767, '\n');
			std::cout << "Неверный формат ввода. Выберите \"+\", \"-\" \"*\" или \"/\".\n";
		}
	}
}

void printResult(double a, char sm, double b)
{
	switch (sm) {
	case '+':
		std::cout << a + b;
		break;
	case '-':
		std::cout << a - b;
		break;
	case '*':
		std::cout << a * b;
		break;
	case '/':
		std::cout << a / b;
		break;
	default:
		std::cout << "Неверный формат ввода.";
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