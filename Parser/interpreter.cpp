#include "interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::exec(std::string command) {

	std::stringstream ss(command);
	std::string code;
	ss >> code;

	if (code == "ldc") {  // Load integer constant onto stack
		std::string arg1;
		ss >> arg1;
		stack.push_back(std::atoi(arg1.c_str()));
	}
	else if (code == "iadd") {
		int b = stack.back();
		stack.pop_back();
		int a = stack.back();
		stack.pop_back();
		stack.push_back(a + b);
	}
	else if (code == "isub") {
		int b = stack.back();
		stack.pop_back();
		int a = stack.back();
		stack.pop_back();
		stack.push_back(a - b);
	}
	else if (code == "imul") {
		int b = stack.back();
		stack.pop_back();
		int a = stack.back();
		stack.pop_back();
		stack.push_back(a * b);
	}
	else if (code == "idiv") {
		int b = stack.back();
		stack.pop_back();
		int a = stack.back();
		stack.pop_back();
		stack.push_back(a / b);
	}
	else if (code == "istore") {  // Store an int value in localVars.
		int a = stack.back();
		stack.pop_back();
		localVars.push_back(a);
	}
	else if (code == "iset") {  // Set a pre-existing int value in localVars.
		std::string arg1;  // Which value to set
		ss >> arg1;
		int a = stack.back();
		stack.pop_back();
		localVars[std::atoi(arg1.c_str())] = a;
	}
	else if (code == "iload") {
		std::string arg1;  // Which index to get int value of
		ss >> arg1;
		stack.push_back(localVars[std::atoi(arg1.c_str())]);
	}
	else {
		throw std::runtime_error("Invalid command '" + code + "'!");
	}
}

std::string Interpreter::getStackDump() {
	std::string s;
	for (unsigned int index = 0; index < stack.size(); ++index) {
		s += std::to_string(stack[index]) + "\n";
	}
	return s;
}

std::string Interpreter::getVarsDump() {
	std::string s;
	for (unsigned int index = 0; index < localVars.size(); ++index) {
		s += std::to_string(localVars[index]) + "\n";
	}
	return s;
}
