#include <iostream>
#include <fstream>

#include "interpreter.h"

int main(int argc, char* argv[]) {
	
	Interpreter interp;
	
	try {
		std::ifstream file("../programs/program.compiled");
		std::string line;
		while (std::getline(file, line)) {
			interp.exec(line);
			std::cout << "Executed " << line << ". Stack:" << std::endl << interp.getStackDump();
		}
	}
	catch (std::runtime_error& ex) {
		std::cout << ex.what() << std::endl;
	}

	system("PAUSE");
}