#pragma once

#include <string>

union StackItem {
	int intval;
	double doubleval;
	float floatval;
	bool boolval;

	StackItem(unsigned int i) : intval(static_cast<int>(i)) {}
	StackItem(int i) : intval(i) {}
	StackItem(double d) : doubleval(d) {}
	StackItem(float f) : floatval(f) {}
	StackItem(bool b) : boolval(b) {}

	std::string str() {
		return "i(" + std::to_string(intval) + ")\td(" + std::to_string(doubleval) + ")\tf(" + std::to_string(floatval) + ")\tb(" + (boolval ? "true" : "false") + ")";
	}
};
