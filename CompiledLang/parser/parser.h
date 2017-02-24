#pragma once

#include "../tokeniser/tokeniser.h"
#include "node.h"

class Parser {

private:
	std::vector<Token> tokens;
	unsigned int index;

public:
	Parser(std::vector<Token> tokens);
	Node* parse();

private:
	Token token() { return token(index); }
	Token token(unsigned int index) { return (index < tokens.size()) ? tokens[index] : Token("<NULL>", static_cast<TokenType>(0)); }

	void eat(TokenType type) {
		if (token().type != type)
			throw std::runtime_error("Token type " + tokenTypeString(token().type) + " was unexpected, expected " + tokenTypeString(type) + ".");
		++index;
	}

	Node* parseNumber();
	Node* parseArithmeticUnit();
	Node* parseAddSub();
	Node* parseMulDiv();
	Node* parseArithmeticExpression();
	Node* parseName(NodeName::Usage usage);
	Node* parseDeclareAssign();

	Node* parseStatement();
	Node* parseProgram();
};