﻿#include <iostream>
#include <cmath>
#include <vector>

template<class K>
using nvect = std::vector<K>;
template<class K>
using nmatr = std::vector<std::vector<K>>;

template<class K>
bool sort(nvect<K> arr, bool increase = true) {
    int arr_len = arr.size();
    if (increase) {
        for (int i = 1; i < arr_len; i++) {

            if (arr[i - 1] > arr[i]) {
                return false;
            }

        }
    }
    else {
        for (int i = 1; i < arr_len; i++) {
            if (arr[i - 1] < arr[i]) {
                return false;
            }

        }
    }
    return true;
}

template<class K>
nvect<K> BozoSort(nvect<K> arr, bool increase = true) {
    int arr_len = arr.size();
    while (!sort(arr, increase)) {
        int first = rand() % arr_len;
        int second = rand() % arr_len;
        std::swap(arr[first], arr[second]);
    }
    return arr;
}

template<class K>
nvect<K> BozoSort(nmatr<K> arr, bool increase = true) {
    nvect<K> list1;
    for (nvect<K> i : arr) {
        for (K j : i) {
            list1.push_back(j);
        }
    }
    return BozoSort(list1, increase);
}

template<class K>
nvect<K> BozoSort(K first, K second, K third, bool increase = true) {
    nvect<K> els;
    els = {first, second, third};
    return BozoSort(els, increase);
}

template<class K>
std::pair<nvect<K>, nmatr<K>> get_nums()
{
    int len;
    std::cout << "Введите число: ";
    std::cin >> len;
    nmatr<K> matr;
    nvect<K> vect;
    nvect<K> temp;

    std::cout << "Введите эл-ты массива через пробел: ";
    for (int i = 1; i <= len; i++)
    {
        K element;
        std::cin >> element;
        temp.push_back(element);
        vect.push_back(element);
        if (i % int(sqrt(len)) == 0)
        {
            matr.push_back(temp);
            temp.clear();
        }
    }
    return {vect, matr};
}

template<class K>
void printv(nvect<K> arr) {
    for (K i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

int main()
{
    setlocale(LC_ALL, "ru");

    auto doubl = get_nums<double>();
    nvect<double> dvect = doubl.first;
    nmatr<double> dmatr = doubl.second;

    printv(BozoSort(dvect));
    printv(BozoSort(dvect, false));
    printv(BozoSort(dmatr));
    printv(BozoSort(dmatr, false));
    printv(BozoSort(dvect[0], dvect[1], dvect[2]));
    printv(BozoSort(dvect[0], dvect[1], dvect[2], false));

    auto str = get_nums<std::string>();
    nvect<std::string> svect = str.first;
    nmatr<std::string> smatr = str.second;

    printv(BozoSort(svect));
    printv(BozoSort(svect, false));
    printv(BozoSort(smatr));
    printv(BozoSort(smatr, false));
    printv(BozoSort(svect[0], svect[1], svect[2]));
    printv(BozoSort(svect[0], svect[1], svect[2], false));
}