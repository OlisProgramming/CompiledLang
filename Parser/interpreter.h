#pragma once

#include <deque>
#include <string>
#include <sstream>
#include <vector>

class Interpreter {

	std::deque<int> stack;
	std::vector<int> localVars;

public:
	Interpreter();
	void exec(std::string command);
	std::string getStackDump();
	std::string getVarsDump();
};