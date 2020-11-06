#include <iostream>

int main()
{
	setlocale(LC_ALL, "rus");
	int a, i, factorial = 2;
	std::cout << "Введите число: ";
	std::cin >> a;
	if (a == 1 || a == 2) {
		std::cout << a;
		exit(1);
	}
	for (i = 3; i <= a; i++) {
		factorial *= i;
	}
	std::cout << factorial;
}
