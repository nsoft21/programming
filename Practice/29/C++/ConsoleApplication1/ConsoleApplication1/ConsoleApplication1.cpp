#include <iostream>
#include <map>
#include <vector>

typedef std::map<std::string, int> exams_map;
typedef std::vector<std::string> new_vector;

#include <iostream>
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
    els = { first, second, third };
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
    return { vect, matr };
}

template<class K>
void printv(nvect<K> arr) {
    for (K i : arr) {
        std::cout << i << " ";
    }
    std::cout << std::endl;
}

struct Student {
	std::string name;
	int group;
	exams_map exams;
};

int main()
{
	setlocale(LC_ALL, "RUS");
	
	Student s[10] = {};
	new_vector names = { "sd","sf","sg","sh","sj","sk","sl","sa","ss","sq" };
	
	for (int i = 0; i < 10; i++) {
		s[i].name = names[i];
		s[i].group = 1;
		s[i].exams["math"] = 2;
	}
	
	
}