#pragma once
#include <vector>
#include "CustomException.h"

using namespace std;

class DaThuc {
public:
	DaThuc() {

	}
	DaThuc(const DaThuc &b) {
		int len = b.daThuc.size();
		daThuc.resize(len);
		for (int i = 0; i < len; i++) {
			daThuc[i] = b.daThuc[i];
		}
	}
	~DaThuc() {

	}
	int Bac() {
		while (daThuc[daThuc.size() - 1] == 0) {
			daThuc.pop_back();
		}
		return daThuc.size();
	}
	double& GiaTri(int index) {
		if (index >= Bac()) {
			for (int i = 0; i < index - Bac() + 1; i++) {
				daThuc.push_back(0);
			}
		}
		return daThuc[index];
	}
	double& operator()(int index) {
		return GiaTri(index);
	}
	DaThuc operator+(DaThuc &b) {
		int len = b.Bac() > Bac() ? b.Bac() : Bac();
		DaThuc c;
		for (int i = 0; i < len; i++) {
			c(i) = GiaTri(i) + b(i);
		}
		c.Bac();
		return c;
	}
	DaThuc operator-() {
		DaThuc c;
		for (int i = 0; i < Bac(); i++) {
			c(i) = -GiaTri(i);
		}
		return c;
	}
	DaThuc operator-(DaThuc &b) {
		DaThuc c;
		int len = b.Bac() > Bac() ? b.Bac() : Bac();
		for (int i = 0; i < len; i++) {
			c(i) = GiaTri(i) - b(i);
		}
		return c;
	}
	DaThuc operator*(const double &b) {
		for (int i = 0; i < Bac(); i++) {
			GiaTri(i) *= b;
		}
	}
	DaThuc operator/(const double &b) {
		for (int i = 0; i < Bac(); i++) {
			GiaTri(i) /= b;
		}
	}
	DaThuc NhanVoiDaThucBacMot(DaThuc b) {
		if (b.Bac() > 1) {
			throw CustomException("Khong phai da thuc bac 1");
		}
		if (b(1)==0) {
			return *this*b(0);
		}

	}
private:
	vector<double> daThuc;
};