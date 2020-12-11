#include <iostream>
#include <map>

typedef std::map<int, int> new_Map;

int main()
{
	setlocale(LC_ALL, "RUS");
	int num;
	std::cout << "Введите число: ";
	std::cin >> num;

	new_Map arr;
	while (num > 1) {
		for (int i = 2; i <= num; i++) {
			if (num % i == 0) {
				num /= i;
				arr[i] = arr[i] + 1;
				break;
			}
		}
	}

	int len_arr = arr.size();
	int i = 0;
	for (auto el : arr) {
		i++;
		std::cout << el.first;
		if (el.second > 1) {
			std::cout << "^" << el.second;
		}
		if (i != len_arr) {
			std::cout << "*";
		}
	}
}