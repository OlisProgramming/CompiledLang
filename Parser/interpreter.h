#pragma once

#include <deque>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>
#include "stackitem.h"

class Interpreter {

	std::deque<StackItem> stack;
	std::deque<unsigned int> frames;

public:
	Interpreter();
	void exec(std::string command, unsigned int* commandIndex);
	std::string getStackDump();
};