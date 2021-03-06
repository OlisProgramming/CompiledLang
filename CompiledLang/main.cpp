#include <iostream>
#include <fstream>

#include "tokeniser\tokeniser.h"
#include "parser\parser.h"
#include "dependencyresolver\dependencyresolver.h"
#include "typechecker\typechecker.h"
#include "obfuscator\obfuscator.h"
#include "translator\translator.h"

int main(int argc, char* argv[]) {

	try {
		std::string programname = "program";

		std::ifstream t("../programs/" + programname + ".txt");
		std::string str;

		t.seekg(0, std::ios::end);
		str.reserve(static_cast<unsigned int>(t.tellg()));
		t.seekg(0, std::ios::beg);

		str.assign((std::istreambuf_iterator<char>(t)),
			std::istreambuf_iterator<char>());

		Tokeniser tk("../programs/" + programname + ".txt", str);
		auto tokens = tk.tokenise();
		for (Token token : tokens)
			std::cout << token.str() << std::endl;
		
		std::cout << std::endl << std::endl << std::endl;
		
		Parser ps(tokens);
		auto tree = ps.parse();
		tree->print();

		//std::cout << std::endl << std::endl << std::endl;

		auto dependencies = resolveDependencies();

		std::cout << std::endl << std::endl << std::endl;

		checkTypes(tree, std::unordered_map<std::string, DataType>(), dependencies);
		tree->print();

		std::cout << std::endl << std::endl << std::endl;

		obfuscateNames(tree, "../programs/" + programname + ".symbol", dependencies);
		tree->print();

		std::cout << std::endl << std::endl << std::endl;
		
		Translator tr(tree, dependencies);
		std::string out = tr.translate("../programs/" + programname + ".symbol");
		std::ofstream outfile("../programs/" + programname + ".compiled");
		std::cout << out;
		outfile << out;
		delete tree;
		
	}
	catch (std::runtime_error& ex) {
		std::cout << ex.what() << std::endl;
	}

	system("PAUSE");
}