#include "interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::exec(std::string command, unsigned int* commandIndex) {

	std::stringstream ss(command);
	std::string code;
	ss >> code;

	++*commandIndex;

	if (code == "ldc") {  // Load integer constant onto stack
		std::string arg1;
		ss >> arg1;
		stack.push_back(std::atoi(arg1.c_str()));
	}
	else if (code == "goto") {
		unsigned int arg1;
		ss >> arg1;
		stack.push_back(*commandIndex);
		frames.push(stack.size());
		*commandIndex = arg1;
	}
	else if (code == "native") {
		std::string arg1;
		ss >> arg1;
		if (arg1 == "println") {
			int val = stack[stack.size() - 2];  // Arg 0.
			std::cout << "PRINTLN: " << val << std::endl;
			int gotoaddress = stack.back();
			stack.pop_back();
			stack.pop_back();
			stack.push_back(gotoaddress);
		}
	}
	else if (code == "ireturn") {
		int arg1;
		ss >> arg1;
		while (stack.size() > frames.back())
			stack.pop_back();
		frames.pop();
		unsigned int gotoaddress = stack.back();
		*commandIndex = gotoaddress;
		stack.pop_back();
		stack.push_back(arg1);
	}
	else if (code == "frame_alloc") {
		int arg1;
		ss >> arg1;
		frames.push(stack.size());
		for (int i = 0; i < arg1; ++i)
			stack.push_back(0);
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
		int arg1;
		ss >> arg1;
		int a = stack.back();
		stack.pop_back();
		stack[frames.back() + arg1] = a;
	}
	else if (code == "iload") {
		int arg1;  // Which index to get int value of
		ss >> arg1;
		stack.push_back(stack[frames.back() + arg1]);
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
