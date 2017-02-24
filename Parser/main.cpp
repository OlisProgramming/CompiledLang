#include <iostream>
#include <fstream>

#include "interpreter.h"

#define STEP 0

int main(int argc, char* argv[]) {
	
	Interpreter interp;
	
	try {
		std::ifstream file("../programs/program.compiled");
		std::string line;
		while (std::getline(file, line)) {
#if STEP
			std::cout << "Next command: " << line << std::endl;
			system("PAUSE");
			system("CLS");
#endif
			interp.exec(line);
			std::cout << "Executed " << line << ". Stack:" << std::endl << interp.getStackDump();
		}
	}
	catch (std::runtime_error& ex) {
		std::cout << ex.what() << std::endl;
	}

	std::cout << "Finished." << std::endl;
	system("PAUSE");
}