#include <iostream>

int main()
{
	setlocale(LC_ALL, "rus");
	int a, b, i, c;
	std::cout << "Введите число: ";
	std::cin >> a;
	c = a;
	std::cout << "Введите степень числа: ";
	std::cin >> b;
	for (i = 1; i < b; i++) {
		c *= a;
	}
	std::cout << c;
}
