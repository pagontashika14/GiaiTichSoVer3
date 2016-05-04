#pragma once
#include <string.h>

using namespace std;

class CustomException {
public:
	CustomException(char * mes) {
		message = strcpy("", mes);
	}
	string Message() {
		string s = "";
		s.append(message);
		return s;
	}
private:
	char * message;
};
