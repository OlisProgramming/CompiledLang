#include "tokeniser.h"

Tokeniser::Tokeniser(std::string contents) : contents(contents), index(0U) {

}

std::vector<Token> Tokeniser::tokenise() {
	std::vector<Token> tokens;

	while (index < contents.length()) {
		if (isspace(chr())) {
			while (isspace(chr())) {
				++index;
			}
		}
		else {
			tokens.push_back(getNextToken());
		}
	}
	tokens.push_back(Token("", TokenType::FILE_END));

	return tokens;
}

Token Tokeniser::getNextToken() {
		
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

	else if (isalpha(ch)) {
		++index;
		std::string s = std::string(1, ch);
		while (isalpha(chr())) {
			s += chr();
			++index;
		}
		if (s == "int")
			return Token(s, TokenType::INT);
		return Token(s, TokenType::ID);
	}

	else switch (ch) {

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

	case '=':
		++index;
		return Token("=", TokenType::ASSIGN);

	case ';':
		++index;
		return Token(";", TokenType::SEMICOLON);

	default:
		throw std::runtime_error("Invalid token: '" + std::string(1, ch) + "'.");
	}
}
