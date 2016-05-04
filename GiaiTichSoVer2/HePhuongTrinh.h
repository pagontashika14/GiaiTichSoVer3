#pragma once
#include "MaTran.h"
#include "RWText.h"
#include "Nghiem.h"
#include <string>
#include <iostream>
#include <map>
#include "CustomException.h"
#include "Common.h"

using namespace std;

class HePhuongTrinh {
public:
	HePhuongTrinh(MaTran<double> *mt) {
		MT = *mt;
	}

	HePhuongTrinh(char * filePath) {
		RWText txt(filePath);
		MaTran<double> a(txt.RowCount(), txt.ColumnCount() - 1);
		MT.Copy(a);
		MT.MTGhep = new MaTran<double>(txt.RowCount(), 1);
		for (int i = 0; i < txt.RowCount(); i++) {
			for (int j = 0; j < txt.ColumnCount() - 1; j++) {
				MT(i, j) = txt(i, j);
			}
			MT.MTGhep->GiaTri(i, 0) = txt(i, txt.ColumnCount() - 1);
		}
	}

	string ToString(char *format) {

		return MT.ToString(format);
	}
	string ToString(){
		return MT.ToString();
	}

	Nghiem<double> Gauss() {
		MaTran<double> mt = MT.DuaVeMaTranBacThang();
		int sdk0 = mt.SoDongKhacKhongMaTranTo();
		int sdk0A = mt.SoDongKhacKhongMaTranBe();
		if (sdk0 > sdk0A) {
			// Vo nghiem
			Nghiem<double> nghiem(VoNghiem, 0);
			return nghiem;
		}
		else
			if (sdk0 == sdk0A && sdk0 == mt.N()) {
				// Co nghiem duy nhat
				Nghiem<double> nghiem(DuyNhat, sdk0);
				for (int i = sdk0 - 1; i >= 0; i--) {
					nghiem(i, 0) = mt.MTGhep->GiaTri(i, 0);
					for (int j = 0; j < mt.N(); j++) {
						if (mt(i, j) == 0 || j == i) {
							continue;
						}
						nghiem(i, 0) -= mt(i, j) * nghiem(j, 0);
					}
					nghiem(i, 0) /= mt(i, i);
				}
				return nghiem;
			}
			else
				if (sdk0 == sdk0A && sdk0 < mt.N()) {
					// Co vo so nghiem
					Nghiem<double> nghiem(VoSoNghiem, mt.N());
					int dongChon = 0;
					for (int i = 0; i < mt.N(); i++) {
						if (mt(dongChon, i) != 0) {
							nghiem.Loai[i] = PhuThuoc;
							for (int j = dongChon; j < mt.N(); j++) {
								if (j == i) {
									continue;
								}
								nghiem(i, j) = -mt(dongChon, j) / mt(dongChon, i);
							}
							nghiem(i, mt.N()) = mt.MTGhep->GiaTri(dongChon, 0) / mt(dongChon, i);
							dongChon++;
							if (dongChon > mt.M() - 1) {
								break;
							}
						}
					}
					return nghiem;
				}
		return Nghiem<double>(VoNghiem, 0);
	}
	Nghiem<double> GaussJordan() {
		MaTran<double> mt = MT;
		int i = 0, j = 0;
		map<int, int> luuHang;
		for (int m = 0; m < mt.M(); m++) {
			mt.TimPhanTuTroi(luuHang, i, j);
			if (mt(i, j) == 0) {
				break;
			}
			luuHang[i] = j;
			for (int k = 0; k < mt.M(); k++) {
				if (k == i) {
					continue;
				}
				double mtkj = mt(k, j);
				double mtij = mt(i, j);
				for (int l = 0; l < mt.N() + mt.MTGhep->N(); l++) {
					mt(k, l) = mt(k, l) - mt(i, l)*mtkj / mtij;
				}
			}
		}
		// Bien luan
		int sdk0 = mt.SoDongKhacKhongMaTranTo();
		int sdk0A = mt.SoDongKhacKhongMaTranBe();
		if (sdk0 > sdk0A) {
			// Vo nghiem
			Nghiem<double> nghiem(VoNghiem, 0);
			return nghiem;
		}
		else
			if (sdk0 == sdk0A && sdk0 == mt.N()) {
				// Co nghiem duy nhat
				Nghiem<double> nghiem(DuyNhat, sdk0);
				for (map<int, int>::iterator it = luuHang.begin(); it != luuHang.end(); ++it) {
					nghiem(it->second, 0) = mt.MTGhep->GiaTri(it->first, 0) / mt(it->first, it->second);
				}
				return nghiem;
			}
			else
				if (sdk0 == sdk0A && sdk0 < mt.N()) {
					Nghiem<double> nghiem(VoSoNghiem, mt.N());
					for (map<int, int>::iterator it = luuHang.begin(); it != luuHang.end(); ++it) {
						nghiem.Loai[it->second] = PhuThuoc;
						for (int k = 0; k < mt.N(); k++) {
							if (k == it->second) {
								continue;
							}
							nghiem(it->second, k) = -mt(it->first, k) / mt(it->first, it->second);
						}
						nghiem(it->second, mt.N()) = mt.MTGhep->GiaTri(it->first, 0) / mt(it->first, it->second);
					}
					return nghiem;
				}
		return Nghiem<double>(VoNghiem,0);
	}
	Nghiem<double> LapJacobi(double dkDung) {
		MaTran<double> mt = MT;
		if (mt.M() != mt.N()) {
			throw CustomException("Khong phai ma tran vuong");
		}
		if (!mt.IsMaTranCheoTroi()) {
			throw CustomException("Khong phai ma tran cheo troi");
		}
		MaTran<double> a(mt.M(), mt.N());
		MaTran<double> b(mt.M(), 1);
		for (int i = 0; i < a.M(); i++) {
			for (int j = 0; j < a.N(); j++) {
				if (j == i) {
					a(i, j) = 0;
					continue;
				}
				a(i, j) = -mt(i, j) / mt(i, i);
			}
			b(i, 0) = mt.MTGhep->GiaTri(i, 0) / mt(i, i);
		}
		int cstt = Common::ChuSoTinTuong(dkDung);
		dkDung = dkDung - 0.5 * pow(10, -cstt);
		double cvc = a.ChuanVoCung();
		Nghiem<double> x(DuyNhat, mt.M());
		Nghiem<double> x0 = x;
		MaTran<double> x1 = a * (MaTran<double>)x0 + b;
		int soPhepLap = ceill (
			log(dkDung * (1 - cvc) / (x1 - x0).ChuanVoCung())
			/
			log(cvc)
		);
		for (int i = 0; i < soPhepLap; i++) {
			x.Copy(a * x1 + b);
			x1.Copy(x);
		}
		return x;
	}
private:
	MaTran<double> MT;
};



