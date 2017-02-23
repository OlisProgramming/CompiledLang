#pragma once

#include "../parser/node.h"

class Translator {
	
private:
	Node* program;
	std::string output;

public:
	Translator(Node* program) : program(program) {}
	std::string translate() { output = ""; translate(program); return output; }

private:
	void send(std::string str) { output += str + "\n"; }
	void translate(Node* node);
};
