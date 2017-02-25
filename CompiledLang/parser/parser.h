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
	Token peek(unsigned int off = 1U) { return token(index + off); }
	Token token(unsigned int index) { return (index < tokens.size()) ? tokens[index] : Token("<NULL>", static_cast<TokenType>(0), FilePos(0, 0, "")); }

	void eat(TokenType type) {
		if (token().type != type)
			throw std::runtime_error("Token type " + tokenTypeString(token().type) + " was unexpected, expected " + tokenTypeString(type) + ". Error at " + token().pos.str() + ".");
		++index;
	}

	Node* parseNumber();
	Node* parseArithmeticUnit();
	Node* parseAddSub();
	Node* parseMulDiv();
	Node* parseArithmeticExpression();
	Node* parseName(NodeName::Usage usage);

	Node* parseFunctionCall();
	Node* parseAssign();
	Node* parseDeclareAssign();

	Node* parseStatement();
	Node* parseProgram();
};