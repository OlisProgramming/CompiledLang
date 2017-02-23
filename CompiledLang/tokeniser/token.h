#pragma once

#include <string>

enum class TokenType {
	NUMBER,
	ADD, SUB, MUL, DIV,
	LPARENTH, RPARENTH,
};

inline std::string tokenTypeString(TokenType type) {
	switch (type) {
	case TokenType::NUMBER: return "NUMBER";
	case TokenType::ADD: return "ADD";
	case TokenType::SUB: return "SUB";
	case TokenType::MUL: return "MUL";
	case TokenType::DIV: return "DIV";
	case TokenType::LPARENTH: return "LPARENTH";
	case TokenType::RPARENTH: return "RPARENTH";
	}
	return "UNRECOGNISED TOKEN";
}

struct Token {
	std::string contents;
	TokenType type;

	Token(std::string contents, TokenType type) : contents(contents), type(type) {}
	std::string str() { return contents + " (" + std::to_string(static_cast<int>(type)) + ")"; }
};