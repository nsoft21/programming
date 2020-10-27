#include <iostream>

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    std::cout << "Введите числа a и b:\n";
    int a, b, x;
    std::cin >> a >> b;
    x = a;
    a = b;
    b = x;
    std::cout << "\nМеняем значения используя доп переменную: ";
    std::cout << "\na = " << a << "\nb = " << b;
    std::cout << "\n2я замена без доп переменной: ";
    a += b;
    b = a - b;
    a -= b;
    std::cout << "\na = " << a << "\nb = " << b;
}
