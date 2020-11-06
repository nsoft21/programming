#include <iostream>

int main()
{
	setlocale(LC_ALL, "rus");
	int a, b, c;
	std::cout << "Введите целое число: ";
	std::cin >> a;
	b = 0;
	c = 1;
	while (c <= a) {
		c *= 2;
		b += 1;
	}
	std::cout << b << '\n';
}
