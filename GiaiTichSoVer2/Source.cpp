#pragma once
#include <iostream>
#include <conio.h>
#include <string>
#include "RWText.h"
#include "MaTran.h"
#include "HePhuongTrinh.h"
#include <vector>
#include <functional>

using namespace std;

void test(int a,function<void(int&)> f) {
	
	f(a);
}

int main() {
	int a = 1;
	int b = 2;
	test(a,[&](int &a)
	{
		a = b / 2;
	});
	cout << a;
	_getch();
	return 0;
}
