#include "interpreter.h"

Interpreter::Interpreter() {
}

void Interpreter::exec(std::string command, unsigned int* commandIndex) {

	std::stringstream ss(command);
	std::string code;
	ss >> code;

	++*commandIndex;

	if (code == "ildc") {  // Load integer constant onto stack
		int arg1;
		ss >> arg1;
		stack.push_back(arg1);
	}
	else if (code == "dldc") {  // Load double-precision floating point constant onto stack
		double arg1;
		ss >> arg1;
		stack.push_back(arg1);
	}
	else if (code == "fldc") {  // Load floating point constant onto stack
		float arg1;
		ss >> arg1;
		stack.push_back(arg1);
	}
	else if (code == "bldc") {  // Load boolean constant onto stack
		bool arg1;
		ss >> arg1;
		stack.push_back(arg1);
	}


	else if (code == "goto") {
		unsigned int arg1;
		ss >> arg1;
		stack.push_back(*commandIndex);
		frames.push_back(stack.size());
		*commandIndex = arg1;
	}

	else if (code == "i2d") {
		stack.back().doubleval = static_cast<double>(stack.back().intval);
	}
	else if (code == "i2f") {
		stack.back().floatval = static_cast<float>(stack.back().intval);
	}
	else if (code == "i2b") {
		stack.back().boolval = (stack.back().intval != 0);
	}
	else if (code == "d2i") {
		stack.back().intval = static_cast<int>(stack.back().doubleval);
	}
	else if (code == "d2f") {
		stack.back().floatval = static_cast<float>(stack.back().doubleval);
	}
	else if (code == "d2b") {
		stack.back().boolval = (stack.back().doubleval != 0);
	}
	else if (code == "f2i") {
		stack.back().intval = static_cast<int>(stack.back().floatval);
	}
	else if (code == "f2d") {
		stack.back().doubleval = static_cast<double>(stack.back().floatval);
	}
	else if (code == "f2b") {
		stack.back().boolval = (stack.back().floatval != 0);
	}
	else if (code == "b2i") {
		stack.back().intval = (stack.back().boolval ? 1 : 0);
	}
	else if (code == "b2d") {
		stack.back().doubleval = (stack.back().boolval ? 1. : 0.);
	}
	else if (code == "b2f") {
		stack.back().floatval = (stack.back().boolval? 1.f: 0.f);
	}
	
	else if (code == "iprintln") {
		StackItem val = stack[stack.size() - 2];  // Arg 0.
		std::cout << val.intval << std::endl;
		StackItem gotoaddress = stack.back();
		stack.pop_back();
		stack.pop_back();
		stack.push_back(gotoaddress);
	}
	else if (code == "dprintln") {
		StackItem val = stack[stack.size() - 2];  // Arg 0.
		std::cout << val.doubleval << std::endl;
		StackItem gotoaddress = stack.back();
		stack.pop_back();
		stack.pop_back();
		stack.push_back(gotoaddress);
	}
	else if (code == "fprintln") {
		StackItem val = stack[stack.size() - 2];  // Arg 0.
		std::cout << val.floatval << std::endl;
		StackItem gotoaddress = stack.back();
		stack.pop_back();
		stack.pop_back();
		stack.push_back(gotoaddress);
	}
	else if (code == "bprintln") {
		StackItem val = stack[stack.size() - 2];  // Arg 0.
		std::cout << (val.boolval? "true" : "false") << std::endl;
		StackItem gotoaddress = stack.back();
		stack.pop_back();
		stack.pop_back();
		stack.push_back(gotoaddress);
	}

	else if (code == "return") {  // return nothing to stack
		while (stack.size() > frames.back())
			stack.pop_back();
		frames.pop_back();
		StackItem gotoaddress = stack.back();
		*commandIndex = gotoaddress.intval;
		stack.pop_back();
	}
	else if (code == "ireturn") {  // Int return
		int arg1;
		ss >> arg1;
		while (stack.size() > frames.back())
			stack.pop_back();
		frames.pop_back();
		StackItem gotoaddress = stack.back();
		*commandIndex = gotoaddress.intval;
		stack.pop_back();
		stack.push_back(arg1);
	}
	else if (code == "sreturn") {  // Stack return
		StackItem returnValue = stack.back();
		while (stack.size() > frames.back())
			stack.pop_back();
		frames.pop_back();
		StackItem gotoaddress = stack.back();
		*commandIndex = gotoaddress.intval;
		stack.pop_back();
		stack.push_back(returnValue);
	}

	else if (code == "frame_alloc") {
		int arg1;
		ss >> arg1;
		//frames.push(stack.size());
		for (int i = 0; i < arg1; ++i)
			stack.push_back(0);
	}

	else if (code == "iadd") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a + b);
	}
	else if (code == "isub") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a - b);
	}
	else if (code == "imul") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a * b);
	}
	else if (code == "idiv") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a / b);
	}
	else if (code == "igt") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a > b);
	}
	else if (code == "ilt") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a < b);
	}
	else if (code == "ige") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a >= b);
	}
	else if (code == "ile") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a <= b);
	}
	else if (code == "ieq") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a == b);
	}
	else if (code == "ine") {
		int b = stack.back().intval;
		stack.pop_back();
		int a = stack.back().intval;
		stack.pop_back();
		stack.push_back(a != b);
	}

	else if (code == "dadd") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a + b);
	}
	else if (code == "dsub") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a - b);
	}
	else if (code == "dmul") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a * b);
	}
	else if (code == "ddiv") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a / b);
	}
	else if (code == "dgt") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a > b);
	}
	else if (code == "dlt") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a < b);
	}
	else if (code == "dge") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a >= b);
	}
	else if (code == "dle") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a <= b);
	}
	else if (code == "deq") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a == b);
	}
	else if (code == "dne") {
		double b = stack.back().doubleval;
		stack.pop_back();
		double a = stack.back().doubleval;
		stack.pop_back();
		stack.push_back(a != b);
	}

	else if (code == "fadd") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a + b);
	}
	else if (code == "fsub") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a - b);
	}
	else if (code == "fmul") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a * b);
	}
	else if (code == "fdiv") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a / b);
	}
	else if (code == "fgt") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a > b);
	}
	else if (code == "flt") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a < b);
	}
	else if (code == "fge") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a >= b);
	}
	else if (code == "fle") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a <= b);
	}
	else if (code == "feq") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a == b);
	}
	else if (code == "fne") {
		float b = stack.back().floatval;
		stack.pop_back();
		float a = stack.back().floatval;
		stack.pop_back();
		stack.push_back(a != b);
	}

	else if (code == "lognot") {
		stack.back().boolval = !(stack.back().boolval);
	}

	else if (code == "istore" || code == "dstore" || code == "fstore" || code == "bstore") {  // Store a value in localVars.
		int arg1;
		ss >> arg1;
		StackItem a = stack.back();
		stack.pop_back();
		stack[frames.back() + arg1] = a;
	}
	else if (code == "iload" || code == "dload" || code == "fload" || code == "bload") {
		int arg1;  // Which index to get value of
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
		s += stack[index].str() + "\n";
	}
	return s;
}
