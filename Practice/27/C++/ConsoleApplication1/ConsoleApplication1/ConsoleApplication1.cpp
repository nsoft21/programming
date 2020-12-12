#include <iostream>
#include <vector>

typedef std::vector<int> nvect;

int main() {
    setlocale(LC_ALL, "ru");
    nvect vect;

    int n, el;
    std::cout << "Введите кол-во эл-ов: ";
    std::cin >> n;
    std::cout << "Введите эл-ты массива через пробел: ";
    std::cin >> el;
    vect = {el};
    std::cout << vect[0] << std::endl;

    for (int i = 1; i < n; i++) {
        std::cin >> el;

        int j = 0;
        for (j = 0; j < vect.size(); j++) {
            if (el > vect[j]) {
                vect.insert(vect.begin() + j, el);
                break;
            }
        }

        if (j == 5 or j == vect.size()) {
            vect.push_back(el);
        }
        if (vect.size() > 5) {
            vect.erase(vect.begin());
        }

        for (int i : vect) {
            std::cout << i << " ";
        }
        std::cout << std::endl;
    }
}