#include <iostream>

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    std::cout << "Введите числа x(0), v(0) и t:\n";
    float x, v, t, a;
    a = 9.8;
    std::cin >> x >> v >> t;
    std::cout << "x(t) = " << x+v*t-a*t*t/2;
    std::cout << "\n2й случай: x(t) = " << x + v * t - 1/2*a*t*t; //Вывод только x+vt, т.к. 1/2 округляется до 0 (иначе нужен float)
}
