#pragma once
#include "MaTran.h"
using namespace std;

class DaThucNoiSuy {
public:
	DaThucNoiSuy(char *file) {
		MaTran<double> mt(file);
		if (mt.M() == 2) {
			bangSo = mt;
		}
		else {
			bangSo = mt.MaTranChuyenVi();
		}
	}
	string ToString() {
		return bangSo.ToString();
	}
	
private:
	MaTran<double> bangSo;
};
