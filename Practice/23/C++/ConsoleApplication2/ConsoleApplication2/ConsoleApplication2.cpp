#include <iostream>

int fact(int num);

int main()
{
    setlocale(LC_ALL, "rus");
    int num;
    std::cout << "Введите число: ";
    std::cin >> num;
    num = fact(num);
    std::cout << num;
}
