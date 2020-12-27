#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <ctime>

typedef std::map<std::string, int> exams_map;
typedef std::vector<std::string> new_vector;

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
    nmatr<K> matr;
    nvect<K> vect;
    nvect<K> temp;

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

struct Student
{
    std::map<std::string, int> exams_st;
    std::string name_st;
    int group_st;

    Student(std::string name, int group,
        int math, int phys, int hist, int prog)
    {
        if (group < 1 or group > 9)
            throw std::invalid_argument("Error group: " + std::to_string(group));
        if (math < 0 or math > 9 or phys < 0 or phys > 9 or hist < 0 or hist > 9
            or prog < 0 or prog > 9)
            throw std::invalid_argument("Error note: " +
                std::to_string(math) + ", " + std::to_string(phys) + ", " + std::to_string(hist)
                + ", " + std::to_string(prog));
        name_st = name;
        group_st = group;
        exams_st = { {"math", math}, {"phys", phys}, {"hist", hist}, {"prog", prog} };
    }

    friend
        std::ostream& operator << (std::ostream& out, const Student& student)
    {
        auto name_len = std::max(int(student.name_st.length()), 4);
        std::string devider; for (int i = 0; i < name_len; ++i) devider += "─";
        out << "┌─" << devider << "─┬───────┬──────┬──────┬──────┬──────┐\n"
            << "│ Name " << std::string(name_len - 4, ' ')
            << "│ Group │ Math │ Phys │ Hist │ Prog │\n"
            << "├─" << devider << "─┼───────┼──────┼──────┼──────┼──────┤\n"
            << "│ " << student.name_st << " │ " << student.group_st
            << "     │ " << student.exams_st.at("math")
            << "    │ " << student.exams_st.at("phys")
            << "    │ " << student.exams_st.at("hist")
            << "    │ " << student.exams_st.at("prog")
            << "    │\n"
            << "└─" << devider << "─┴───────┴──────┴──────┴──────┴──────┘\n";
        return out;
    }
    bool operator < (const Student& other) const
    {
        return name_st < other.name_st;
    }
    bool operator > (const Student& other) const
    {
        return name_st > other.name_st;
    }
};

std::ostream& operator << (std::ostream& out, const std::vector<Student>& student_vec)
{
    auto name_len = student_vec.at(0).name_st.length();
    for (auto itr = student_vec.begin() + 1; itr != student_vec.end(); ++itr)
    {
        if (itr->name_st.length() > name_len) name_len = itr->name_st.length();
    }
    std::string devider; for (int i = 0; i < name_len; ++i) devider += "─";
    out << "┌─" << devider << "─┬───────┬──────┬──────┬──────┬──────┐\n"
        << "│ Name " << std::string(name_len - 4, ' ')
        << "│ Group │ Math │ Phys │ Hist │ Prog │\n"
        << "├─" << devider << "─┼───────┼──────┼──────┼──────┼──────┤";

    for (auto& student : student_vec) {
        out << "\n│ " << student.name_st
            << std::string(name_len - student.name_st.length(), ' ')
            << " │ " << student.group_st
            << "     │ " << student.exams_st.at("math")
            << "    │ " << student.exams_st.at("phys")
            << "    │ " << student.exams_st.at("hist")
            << "    │ " << student.exams_st.at("prog")
            << "    │\n"
            << "├─" << devider << "─┼───────┼──────┼──────┼──────┼──────┤";
    }
    out << "\r" << "└─" << devider << "─┴───────┴──────┴──────┴──────┴──────┘\n";
    return out;
};

int main()
{
    setlocale(LC_ALL, "RUS");
    std::srand(std::time(0));
    std::vector<Student> students = {
        Student("Петров А",     5, 2, 5, 4, 5),
        Student("Щеглов Н",    5, 3, 3, 5, 3),
        Student("Иванов С",     5, 4, 5, 4, 5),
        Student("Поребин Г",    5, 2, 3, 5, 2),
        Student("Белый А",      5, 3, 5, 4, 3),
        Student("Григорьев П",       5, 4, 3, 5, 5),
        Student("Лобанов Е",    5, 2, 5, 4, 2),
        Student("Цукер М",      5, 3, 3, 5, 3),
        Student("Путя Р",      5, 4, 5, 4, 5),
        Student("Колобок В",  5, 2, 3, 5, 2),
    };

    auto has_failed = [](Student& student) -> bool {
        for (auto& pair : student.exams_st)
        {
            if (pair.second == 2) return true;
        }
        return false;
    };

    std::vector<Student> failed;
    for (auto& student : students)
    {
        if (has_failed(student)) failed.push_back(student);
    }
    if (failed.size() > 0)
    {
        std::cout << failed << std::endl;
        std::cout << "Explusion\n" << failed.at(std::rand() % failed.size()) << std::endl;
    }
    else
    {
        std::cout << "Not found" << std::endl;
    }
}