#pragma once

#include <unordered_map>
#include "../parser/node.h"

class Translator {
	
private:
	Node* program;
	std::unordered_map<std::string, FunctionPointer>& dependencies;
	std::unordered_map<std::string, unsigned int> dependencyLineNumbers;
	std::string output;
	unsigned int currentLine;

public:
	Translator(Node* program, std::unordered_map<std::string, FunctionPointer>& dependencies) : program(program), dependencies(dependencies) {}
	std::string translate();

private:
	void send(std::string str) { output += str + "\n"; ++currentLine; }
	void translate(Node* node);
};
