#include "pch.h"
#include <iostream>
#include <math.h>
#include <string>
#include <stdlib.h>


using namespace std;

int getTime(string s){
	string a, b;
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
	string s1, s2;
	int a, b;
	cin >> s1;
	cin >> s2;
	a = getTime(s1);
	b = getTime(s2);
	if (abs(a - b) <= 15) cout << "Встреча состоится.";
	else cout << "Встреча не состоится.";
}


	
