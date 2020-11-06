#include <iostream>
#include <math.h>

double correctness(char a)
//Проверка на корректность ввода
{
	while (true)
	{
		std::cout << "Введите число: " << a << " : ";
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
			std::cin.ignore(32767, '\n');
			return a;
		}
	}
}

float method1() {
	double a, b, c, p, S;
	a = correctness('a');
	b = correctness('b');
	c = correctness('c');
	p = (a + b + c) / 2;
	S = sqrt(p * (p - a) * (p - b) * (p - c));
	std::cout << "S = " << S << "\n";
	return 0;
}

float method2() {
	double x1, y1, x2, y2, x3, y3, A;
	std::cout << "Введите три пары вещественных чисел, каждая пара в отдельной строке. \n";
	std::cin >> x1 >> y1;
	std::cin >> x2 >> y2;
	std::cin >> x3 >> y3;
	A = abs(x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0;
	std::cout << "S = " << A << "\n";
	return 0;
}

int main()
{
	setlocale(LC_ALL, "rus");
	while (true) {
		std::cout << "Выберите способ ввода параметров треугольника:\n"
			<< "\tВведите 1, если хотите ввести длины сторон a, b, c;\n"
			<< "\tВведите 2, если хотите ввести координаты вершин A, B, C;\n";
		int input_method;
		std::cin >> input_method;
		if (std::cin.fail()) {
			std::cin.clear();
			std::cin.ignore(32767, '\n');
		}
		else if (input_method == 1) {
			std::cin.ignore(32767, '\n');
			method1();
			return 0;
		}
		else if (input_method == 2) {
			std::cin.ignore(32767, '\n');
			method2();
			return 0;
		}
	}
}