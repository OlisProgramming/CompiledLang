#include <iostream>
#include <fstream>
#include <rlutil.h>

#include "interpreter.h"

#define INTERPRETER_DEBUG 0

int main(int argc, char* argv[]) {
	
	Interpreter interp;
	rlutil::setColor(rlutil::GREY);

	try {
		std::ifstream file("../programs/program.compiled");
		std::string line;
		std::vector<std::string> lines;
		while (std::getline(file, line)) {
			lines.push_back(line);
		}

		unsigned int commandIndex = 0U;
		while (commandIndex < lines.size()) {
#if INTERPRETER_DEBUG
			rlutil::setColor(rlutil::LIGHTMAGENTA);
			std::cout << "Executing command " << commandIndex << " (" << lines[commandIndex] << ")" << std::endl;
			rlutil::setColor(rlutil::WHITE);
#endif
			interp.exec(lines[commandIndex], &commandIndex);
#if INTERPRETER_DEBUG
			rlutil::setColor(rlutil::GREY);
			std::cout << "Next is " << commandIndex << ". Stack:" << std::endl << interp.getStackDump();
#endif
		}
	}
	catch (std::runtime_error& ex) {
		std::cout << std::endl << ex.what() << std::endl;
	}

	std::cout << "Finished." << std::endl;
	system("PAUSE");
}