#pragma once
#include "MaTran.h"
#include <string>

using namespace std;

enum NghiemStatus {
	DuyNhat,
	VoNghiem,
	VoSoNghiem,
};
enum LoaiNghiem {
	TuDo,
	PhuThuoc,
};

template<class T>
class Nghiem : public MaTran<T> {
public:
	Nghiem(NghiemStatus status, int length) : Status(status) {
		if (status == DuyNhat) {
			this->newMaTran(length, 1);
		}
		else if (status == VoSoNghiem) {
			this->newMaTran(length, length + 1);
			Loai = new LoaiNghiem[length];
				for (int i = 0; i < length; i++) {
					Loai[i] = TuDo;
				}
		}
	}
	Nghiem(MaTran<T> &mt) {
		this->Copy(mt);
		if (mt.M() != 0) {
			if (mt.N() > 1) {
				Status = VoSoNghiem;
			}
			else {
				Status = DuyNhat;
			}
		}
	}
	Nghiem(const Nghiem<T> &B) {
		this->Copy(B);
		Status = B.Status;
	}
	NghiemStatus Status;
	LoaiNghiem *Loai;

	string ToString() {
		string s = "";
		switch (Status) {
			default:
				return "";
			case VoNghiem:
				return "He vo nghiem.";
			case DuyNhat:
				s = "He co nghiem duy nhat:\n";
				for (int i = 0; i < this->m; i++) {
					char str[200];
					sprintf(str, "x[%d] = %-10.6f\n", i, this->GiaTri(i, 0));
					s.append(str);
				}
				return s;
			case VoSoNghiem:
				s = "He co vo so nghiem:\n";
				char str[200];
				for (int i = 0; i < this->m; i++) {
					if (Loai[i] == TuDo) {
						sprintf(str, "x[%d] tu do\n", i);
						s.append(str);
					}
					else {
						sprintf(str, "x[%d] = ", i);
						s.append(str);
						T bi = this->GiaTri(i,this->m);
						if (bi != 0) {
							sprintf(str, "%.6f ", bi);
							s.append(str);
						}
						for (int j = 0; j < this->m; j++) {
							if (this->GiaTri(i,j) > 0) {
								sprintf(str, "+ %.6f.x[%d] ", this->GiaTri(i, j), j);
								s.append(str);
							}
							else if (this->GiaTri(i,j) < 0) {
								sprintf(str, "- %.6f.x[%d] ", -this->GiaTri(i, j), j);
								s.append(str);
							}
						}
						s.append("\n");
					}
				}
				return s;
		}
	}
private:

};
