
#include <iostream>

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    std::cout << "Введите числа a и b:\n";
    int a, b;
    double c, d;
    std::cin >> a >> b;
    c = a;
    d = b;
    std::cout << "int:int";
    std::cout << "\na + b = " << a+b << "\na - b = " << a-b << "\na * b = " << a*b << "\na / b = " << a/b;
    std::cout << "\ndouble:double";
    std::cout << "\na + b = " << c + d << "\na - b = " << c - d << "\na * b = " << c * d << "\na / b = " << c / d;
    std::cout << "\nint:double";
    std::cout << "\na + b = " << a + d << "\na - b = " << a - d << "\na * b = " << a * d << "\na / b = " << a / d;
    std::cout << "\ndouble:int";
    std::cout << "\na + b = " << c + b << "\na - b = " << c - b << "\na * b = " << c * b << "\na / b = " << c / b;
}
