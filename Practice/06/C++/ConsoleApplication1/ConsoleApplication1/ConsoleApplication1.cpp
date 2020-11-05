#include <iostream>
#include <math.h>

double correctness() {
    //Проверяем на корректность ввода
    double a;
    
    while (true) {
        std::cin >> a;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(32767, '\n');
            std::cout << "Некорректное значение, должно быть число!\n";
        }
        else return a;
    }
}

int main()
{
    setlocale(LC_ALL, "RUSSIAN");
    std::cout << "Введите числа a, b и c:\n";
    double a, b, c, x1, x2, d;
    a = correctness();
    b = correctness();
    c = correctness();

    if (a == 0) {
        x1 = -c / b;
        std::cout << "\nx = " << x1;
    }
    else {
        d = b * b - 4 * a * c;
        if (d > 0) {
            x1 = (-b + sqrt(d)) / (2 * a);
            x2 = (-b - sqrt(d)) / (2 * a);
            if (x1 == x2) std::cout << "\nx = " << x1;
            else std::cout << "\nx1 = " << x1 << "\nx2 = " << x2;
        }
        else {
            std::cout << "\nДействительных корней нет.";
        }
    }

    
}

