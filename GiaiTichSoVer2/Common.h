#pragma once
#include <math.h>

using namespace std;

class Common {
public: 
	static int ChuSoTinTuong(double dk) {
		int i = 0;
		while (dk < 0.5 * pow(10, -i)) {
			i++;
		}
		return i;
	}
};