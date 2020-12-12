#include <iostream>
#include <vector>
#include <cmath>
#include <string>

typedef std::vector<int> nvect;
typedef std::vector<std::vector<int>> nmatr;

bool sort(nvect arr, bool increase = true) {
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

nvect BozoSort(nvect arr, bool increase = true) {
	int arr_len = arr.size();
	while (!sort(arr, increase)) {
		int first = rand() % arr_len;
		int second = rand() % arr_len;
		std::swap(arr[first], arr[second]);
	}
	return arr;
}

nvect BozoSort(nmatr arr, bool increase = true) {
	nvect vect1;
	for (nvect i : arr) {
		for (int j : i) {
			vect1.push_back(j);
		}
	}
	return BozoSort(vect1, increase);
}

nvect BozoSort(int first, int second, int third, bool increase = true) {
	nvect els;
	els = { first, second, third };
	return BozoSort(els, increase);
}

void printv(nvect arr) {
	for (int i : arr) {
		std::cout << i << " ";
	}
	std::cout << std::endl;
}

int main()
{
	setlocale(LC_ALL, "ru");
	
	int n, k = 0;
	nmatr matr;
	nvect vect, temp;

	std::cout << "Введите число: ";
	std::cin >> n;
	std::cout << "Введите эл-ты массива через пробел: ";
	for (int i = 1; i <= n; i++)
	{
		int num1;
		std::cin >> num1;
		temp.push_back(num1);
		vect.push_back(num1);
		if (i % int(std::sqrt(n)) == 0)
		{
			matr.push_back(temp);
			temp.clear();
		}
	}

printv(BozoSort(vect));
printv(BozoSort(vect, false));
printv(BozoSort(matr));
printv(BozoSort(matr, false));
printv(BozoSort(vect[0], vect[1], vect[2]));
printv(BozoSort(vect[0], vect[1], vect[2], false));
}