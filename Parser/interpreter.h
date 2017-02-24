#pragma once

#include <queue>
#include <deque>
#include <string>
#include <sstream>
#include <vector>

class Interpreter {

	std::deque<int> stack;
	std::queue<int> frames;

public:
	Interpreter();
	void exec(std::string command);
	std::string getStackDump();
};