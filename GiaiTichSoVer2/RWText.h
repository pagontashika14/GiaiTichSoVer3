#pragma once
#pragma warning (disable : 4996)
#include <string>
#include <cstdlib>

using namespace std;

class RWText {
#define LENGTH 1000
public:
	RWText(int row, int column) :m(row), n(column) {
		data = new double[m*n];
		for (int i = 0; i < m*n; i++) {
			data[i] = 0;
		}
	}
	RWText(char *filePath) {
		file = fopen(filePath, "r");
		if (!file) {
			throw 0;
		}
		data = new double[LENGTH];
		char buf[LENGTH];
		m = 1; n = 0;
		if (fgets(buf, LENGTH, file)) {
			double line[LENGTH];
			int size = 0;
			splitString(line, size, buf);
			n = size;
			for (int i = 0; i < n; i++) {
				data[i] = line[i];
			}
		}
		while (fgets(buf, LENGTH, file)) {
			double line[LENGTH];
			int size = 0;
			splitString(line, size, buf);
			for (int i = 0; i < n; i++) {
				data[m*n + i] = line[i];
			}
			m++;
		}
		fclose(file);
	}
	~RWText() {
		if (file) {
			fclose(file);
		}
		delete[]data;
	}
	string ToString(char * format) {
		string s = "";
		for (int i = 0; i < m; i++) {
			for (int j = 0; j < n; j++) {
				char str2[20];
				double item = operator()(i,j);
				sprintf(str2, format, item);
				s = s.append(str2);
				s = s.append(" ");
			}
			s = s.append("\n");
		}
		return s;
	}
	int RowCount() {
		return m;
	}
	int ColumnCount() {
		return n;
	}
	double& operator()(int i, int j) {
		return data[i*n + j];
	}
	bool SaveToLocation(char * filePath) {
		FILE *f = fopen(filePath, "w+");
		int boo = fprintf(f, ToString("%f ").c_str());
		fclose(f);
		if (boo) {
			return true;
		}
		return false;
	}
private:
	FILE *file;
	double *data;
	int m;
	int n;
	void splitString(double *dest, int &size, char *source) {
		char buf[LENGTH];
		double data[LENGTH];
		int n = 0;
		int count = 0;
		int i;
		for (i = 0; i < LENGTH; i++) {
			if (source[i] == '\n' || source[i] == ' ') {
				buf[n] = '\0';
				n = 0;
				break;
			}
			buf[n] = source[i];
			n++;
		}
		data[0] = atof(buf);
		count = 1;
		for (; i < LENGTH; i++) {
			if (source[i] == '\n' || source[i] == '\0') {
				break;
			}
			if (source[i] == ' ') {
				continue;
			}
			while (source[i] != ' '&&source[i] != '\n'&&source[i] != '\0') {
				buf[n] = source[i];
				n++;
				i++;
			}
			i--;
			buf[n] = '\0';
			data[count] = atof(buf);
			n = 0;
			count++;
		}
		for (int i = 0; i < count; i++) {
			dest[i] = data[i];
		}
		size = count;
	}
};
