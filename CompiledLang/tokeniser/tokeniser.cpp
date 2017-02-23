#include "tokeniser.h"

Tokeniser::Tokeniser(std::string contents) : contents(contents), index(0U) {

}

std::vector<Token> Tokeniser::tokenise() {
	std::vector<Token> tokens;

	while (index < contents.length()) tokens.push_back(getNextToken());

	return tokens;
}

Token Tokeniser::getNextToken() {
	while (isspace(chr())) {
		++index;
	}
	
	char ch = chr();
	if (isdigit(ch)) {
		++index;
		std::string s = std::string(1, ch);
		while (isdigit(chr())) {
			s += chr();
			++index;
		}
		return Token(s, TokenType::NUMBER);
	}
	switch (ch) {

	case '+':
		++index;
		return Token("+", TokenType::ADD);

	case '-':
		++index;
		return Token("-", TokenType::SUB);

	case '*':
		++index;
		return Token("*", TokenType::MUL);

	case '/':
		++index;
		return Token("/", TokenType::DIV);

	case '(':
		++index;
		return Token("(", TokenType::LPARENTH);

	case ')':
		++index;
		return Token(")", TokenType::RPARENTH);

	default:
		throw std::runtime_error("Invalid token: '" + std::string(1, ch) + "'.");
	}
}
