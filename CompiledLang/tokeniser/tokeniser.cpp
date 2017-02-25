#include "tokeniser.h"

Tokeniser::Tokeniser(std::string fname, std::string contents) : fname(fname), contents(contents), index(0U), line(1U), column(0U) {

}

void Tokeniser::advance() {
	++index;
	++column;
	if (chr() == '\n') {
		column = 0;
		++line;
	}
}

FilePos Tokeniser::getPos() {
	return FilePos(line, column, fname);
}

std::vector<Token> Tokeniser::tokenise() {
	std::vector<Token> tokens;

	while (index < contents.length()) {
		if (isspace(chr())) {
			while (isspace(chr())) {
				advance();
			}
		}
		else {
			tokens.push_back(getNextToken());
		}
	}
	tokens.push_back(Token("", TokenType::FILE_END, getPos()));

	return tokens;
}

Token Tokeniser::getNextToken() {
		
	char ch = chr();
	if (isdigit(ch)) {
		advance();
		std::string s = std::string(1, ch);
		bool hasdecimalpoint = false;
		while (isdigit(chr()) || chr() == '.') {
			s += chr();
			if (chr() == '.') {
				if (hasdecimalpoint)
					throw std::runtime_error("Cannot have two decimal points in a number!");
				hasdecimalpoint = true;
			}
			advance();
		}
		if (hasdecimalpoint) {
			if (chr() == 'f') {
				advance();
				return Token(s, TokenType::FLOAT, getPos());
			}
			else {
				return Token(s, TokenType::DOUBLE, getPos());
			}
		}
		else {
			if (chr() == 'f') {
				advance();
				return Token(s, TokenType::FLOAT, getPos());
			}
			else {
				return Token(s, TokenType::INTEGER, getPos());
			}
		}
	}

	else if (isalpha(ch)) {
		advance();
		std::string s = std::string(1, ch);
		while (isalpha(chr())) {
			s += chr();
			advance();
		}
		if (s == "int")
			return Token(s, TokenType::KWD_INT, getPos());
		if (s == "double")
			return Token(s, TokenType::KWD_DOUBLE, getPos());
		if (s == "float")
			return Token(s, TokenType::KWD_FLOAT, getPos());
		if (s == "bool")
			return Token(s, TokenType::KWD_BOOL, getPos());
		if (s == "true")
			return Token(s, TokenType::BOOL, getPos());
		if (s == "false")
			return Token(s, TokenType::BOOL, getPos());
		return Token(s, TokenType::ID, getPos());
	}

	else switch (ch) {

	case '+':
		advance();
		return Token("+", TokenType::ADD, getPos());

	case '-':
		advance();
		if (chr() == '>') {
			advance();
			return Token("->", TokenType::CAST, getPos());
		}
		return Token("-", TokenType::SUB, getPos());

	case '*':
		advance();
		return Token("*", TokenType::MUL, getPos());

	case '/':
		advance();
		return Token("/", TokenType::DIV, getPos());

	case '(':
		advance();
		return Token("(", TokenType::LPARENTH, getPos());

	case ')':
		advance();
		return Token(")", TokenType::RPARENTH, getPos());

	case '=':
		advance();
		if (chr() == '=') {
			advance();
			return Token("==", TokenType::EQ, getPos());
		}
		return Token("=", TokenType::ASSIGN, getPos());

	case ';':
		advance();
		return Token(";", TokenType::SEMICOLON, getPos());

	case '>':
		advance();
		if (chr() == '=') {
			advance();
			return Token(">=", TokenType::GE, getPos());
		}
		return Token(">", TokenType::GT, getPos());

	case '<':
		advance();
		if (chr() == '=') {
			advance();
			return Token("<=", TokenType::LE, getPos());
		}
		return Token("<", TokenType::LT, getPos());

	case '!':
		advance();
		if (chr() == '=') {
			advance();
			return Token("!=", TokenType::NE, getPos());
		}
		return Token("!", TokenType::LOGIC_NOT, getPos());

	default:
		throw std::runtime_error("Invalid token: '" + std::string(1, ch) + "'.");
	}
}
