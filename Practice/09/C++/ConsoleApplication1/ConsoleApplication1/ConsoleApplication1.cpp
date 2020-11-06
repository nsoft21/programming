#include <iostream>

int getTime(std::string s) {
	std::string a, b;
	int hour, minute;
	b = s.substr(s.find(":") + 1);
	minute = atoi(b.c_str());
	a = s.erase(s.find(":"));
	hour = atoi(a.c_str());
	return hour * 60 + minute;
}

int main()
{
	setlocale(LC_ALL, "rus");
	std::string s1, s2;
	int a, b;
	std::cin >> s1;
	std::cin >> s2;
	a = getTime(s1);
	b = getTime(s2);
	if (abs(a - b) <= 15) std::cout << "Встреча состоится.";
	else std::cout << "Встреча не состоится.";
}