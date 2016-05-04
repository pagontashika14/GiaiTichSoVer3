#pragma warning (disable : 4996)
#pragma once
#include <string>
#include <map>
#include <math.h>
#include "RWText.h"
#include "CustomException.h"

using namespace std;

template<class T>
class MaTran {
public:
	MaTran() {
		data = new T[0];
		MTGhep = NULL;
	}
	MaTran(int soHang, int soCot) :m(soHang), n(soCot) {
		data = new T[m*n];
		for (int i = 0;i < m*n;i++) {
			data[i] = 0;
		}
		MTGhep = NULL;
	}
	MaTran(char *file) {
		RWText txt(file);
		newMaTran(txt.RowCount(), txt.ColumnCount());
		for (int i = 0; i < txt.RowCount(); i++) {
			for (int j = 0; j < txt.ColumnCount(); j++) {
				GiaTri(i, j) = txt(i, j);
			}
		}
	}
	MaTran(const MaTran<T> & b) {
		Copy(b);
	}
	void Copy(const MaTran<T> &b) {
		m = b.m;
		n = b.n;
		data = new T[m*n];
		for (int i = 0; i < m*n; i++) {
			data[i] = b.data[i];
		}
		if (b.MTGhep != NULL) {
			int row = b.MTGhep->m;
			int column = b.MTGhep->n;
			MTGhep = new MaTran(row, column);
			MTGhep->Copy(*b.MTGhep);
		}
		else {
			MTGhep = NULL;
		}
	}
	~MaTran() {
		delete[]data;
	}
	MaTran *MTGhep;
	T& operator()(int i, int j) {
		return GiaTri(i, j);
	}
	T& GiaTri(int i, int j) {
		if (j >= n && MTGhep != NULL) {
			return MTGhep->GiaTri(i, j - n);
		}
		return data[i*n + j];
	}
	T Lay(int i,int j) const {
		if (j >= n && MTGhep != NULL) {
			return MTGhep->GiaTri(i, j - n);
		}
		return data[i*n + j];
	}
	string ToString(char *format) {
		string s = "";
		for (int i = 0;i < m;i++) {
			for (int j = 0;j < n;j++) {
				char str[200];
				sprintf(str, format, data[i*n + j]);
				s.append(str);
				s.append(" ");
			}
			if (MTGhep != NULL) {
				s.append(" |  ");
				for (int j = 0; j < MTGhep->n; j++) {
					char str[200];
					sprintf(str, format, MTGhep->operator()(i, j));
					s.append(str);
					s.append(" ");
				}
			}
			s.append("\n");
		}
		return s;
	}
	string ToString() {
		return ToString("%-10.6f");
	}

	int M() const {
		return m;
	}
	int N() const {
		return n;
	}

	MaTran DuaVeMaTranBacThang() {
		MaTran A = *this;
		int cotChon = 0;
		for (int i = 0; i < A.M(); i++) {
			if (cotChon > A.N() - 1) break;
			double bigNum = 0;
			int dong = i;
			for (int j = i; j < A.M(); j++) {
				if (A(j, cotChon) == 1) {
					dong = j;
					break;
				}
				if (fabs(A(j, cotChon)) > bigNum) {
					bigNum = fabs(A(j, cotChon));
					dong = j;
				}
			}
			if (A(dong, cotChon) == 0) {
				cotChon++;
				i--;
				continue;
			}
			else {
				int c = A.MTGhep == NULL?0: A.MTGhep->n;
				for (int k = 0; k < A.n + c; k++) {
					double temp = A(i, k);
					A(i, k) = A(dong, k);
					A(dong, k) = temp;
				}
			}
			for (int j = i + 1; j < A.M(); j++) {
				double ajCotChon = A(j, cotChon);
				double aiCotChon = A(i, cotChon);
				int c = A.MTGhep == NULL ? 0 : A.MTGhep->n;
				for (int k = 0; k < A.n + c; k++) {
					A(j, k) = A(j, k) - A(i, k) * ajCotChon / aiCotChon;
				}
			}
		}
		return A;
	}
	int SoDongKhacKhongMaTranBe() {
		return demSoDongKhacKhong(true);
	}
	int SoDongKhacKhongMaTranTo() {
		return demSoDongKhacKhong(false);
	}
	void TimPhanTuTroi(map<int, int> boQua, int &i, int &j) {
		double bigNum = 0;
		//auto boQuaHang = boQua;
		for (int a = 0; a < m; a++) {
			if (findMap(boQua,a)) {
				continue;
			}
			for (int b = 0; b < n; b++) {
				if (fabs(GiaTri(a,b)) == 1) {
					i = a;
					j = b;
					return;
				}
			}
		}
		for (int a = 0; a < m; a++) {
			if (findMap(boQua,a)) {
				continue;
			}
			for (int b = 0; b < n; b++) {
				if (fabs(GiaTri(a,b)) > bigNum) {
					i = a;
					j = b;
					bigNum = fabs(GiaTri(a,b));
				}
			}
		}
	}
	bool IsMaTranCheoTroi() {
		for (int i = 0; i < m; i++) {
			double line = 0;
			for (int j = 0; j < n; j++) {
				if (j == i) {
					continue;
				}
				line += fabs(GiaTri(i, j));
			}
			if (line >= fabs(GiaTri(i, i))) {
				return false;
			}
		}
		return true;
	}
	double ChuanVoCung() {
		double big = 0;
		for (int i = 0; i < m; i++) {
			double line = 0;
			for (int j = 0; j < n; j++) {
				line += fabs(GiaTri(i, j));
			}
			if (line > big) {
				big = line;
			}
		}
		return big;
	}
	MaTran<T> MaTranChuyenVi() {
		MaTran<T> mtChuyenVi = new MaTran(n, m);
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				mtChuyenVi(i, j) = GiaTri(j, i);
			}
		}
		return mtChuyenVi;
	}
	MaTran<T> operator+(MaTran<T> &B) {
		if (m == B.m && n == B.n) {
			MaTran<T> C(m, n);
			for (int i = 0; i < m; i++) {
				for (int j = 0; j < n; j++) {
					C(i, j) = GiaTri(i, j) + B(i, j);
				}
			}
			return C;
		}
		else {
			throw CustomException("Khong cong duoc");
		}
	}
	MaTran<T> operator*(const MaTran<T> &B) {
		if (N() == B.M()) {
			MaTran<T> C(M(), B.N());
			for (int i = 0; i < C.M(); i++) {
				for (int j = 0; j < C.N(); j++) {
					for (int k = 0; k < N(); k++) {
						C(i, j) += Lay(i, k) * B.Lay(k, j);
					}
				}
			}
			return C;
		}
		else {
			throw CustomException("Khong nhan duoc");
		}
	}
	MaTran<T> operator-() {
		MaTran<double> B = *this;
		for (int i = 0; i < B.m; i++) {
			for (int j = 0; j < B.n; j++) {
				B(i, j) = -B(i, j);
			}
		}
		return B;
	}
	MaTran<T> operator-(MaTran<T> &B) {
		MaTran<double> C(m, n);
		for (int i = 0; i < m*n; i++) {
			C.data[i] = data[i] - B.data[i];
		}
		return C;
	}
protected:
	T *data;
	int m;
	int n;
	void newMaTran(int soHang, int soCot) {
		m = soHang;
		n = soCot;
		data = new T[m*n];
		for (int i = 0;i < m*n;i++) {
			data[i] = 0;
		}
		MTGhep = NULL;
	}
private:
	int demSoDongKhacKhong(bool boo) {
		int dem = 0;
		for (int i = 0; i < m; i++) {
			int tag = 0;
			for (int j = i*n; j < i*n + n;j++) {
				if (data[j] != 0) {
					dem++;
					tag = 1;
					break;
				}
			}
			if (tag == 1 || boo) {
				continue;
			}
			for (int j = i*MTGhep->n; j < (i + 1)*MTGhep->n;j++) {
				if (MTGhep->data[j] != 0) {
					dem++;
					break;
				}
			}
		}
		return dem;
	}
	bool findMap(map<int, int> &m, int &key) {
		for (map<int, int>::iterator it = m.begin(); it != m.end(); ++it) {
			if (it->first==key) {
				return true;
			}
		}
		return false;
	}
};
