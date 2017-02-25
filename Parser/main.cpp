#include <iostream>
#include <fstream>

#include "interpreter.h"

int main(int argc, char* argv[]) {
	
	Interpreter interp;
	
	try {
		std::ifstream file("../programs/program.compiled");
		std::string line;
		std::vector<std::string> lines;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}

		unsigned int commandIndex = 0U;
		while (commandIndex < lines.size()) {
			std::cout << "Executing command " << commandIndex << " (" << lines[commandIndex] << ")";
			interp.exec(lines[commandIndex], &commandIndex);
			std::cout << ". Next is " << commandIndex << ". Stack:" << std::endl << interp.getStackDump();
		}
	}
	catch (std::runtime_error& ex) {
		std::cout << std::endl << ex.what() << std::endl;
	}

	std::cout << "Finished." << std::endl;
	system("PAUSE");
}