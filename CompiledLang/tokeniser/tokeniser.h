#pragma once

#include <string>
#include <iostream>
#include <vector>

#include "token.h"

class Tokeniser {

private:
	std::string contents;
	unsigned int index;

public:
	Tokeniser(std::string contents);
	std::vector<Token> tokenise();

private:
	char chr() { return chr(index); }
	char chr(unsigned int index) { return (index < contents.length()) ? contents[index] : '\0'; }
	Token getNextToken();
};
