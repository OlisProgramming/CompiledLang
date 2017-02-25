#pragma once

#include <deque>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

class Interpreter {

	std::deque<int> stack;
	std::deque<unsigned int> frames;

public:
	Interpreter();
	void exec(std::string command, unsigned int* commandIndex);
	std::string getStackDump();
};