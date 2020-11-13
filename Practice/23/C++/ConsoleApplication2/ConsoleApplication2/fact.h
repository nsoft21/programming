#pragma once
#include <iostream>

int fact(int num) {
	int i, fact = 2;
	if (num == 1 || num == 2) {
		return num;
		exit(1);
	}
	for (i = 3; i <= num; i++) {
		fact *= i;
	}
	return fact;
}