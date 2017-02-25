#pragma once

#include <string>

enum class TokenType {
	NUMBER,
	ADD, SUB, MUL, DIV,
	LPARENTH, RPARENTH,
	ID,
	INT,
	ASSIGN,
	SEMICOLON, COMMA,
	FILE_END
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
	case TokenType::ID: return "ID";
	case TokenType::INT: return "INT";
	case TokenType::ASSIGN: return "ASSIGN";
	case TokenType::SEMICOLON: return "SEMICOLON";
	case TokenType::COMMA: return "COMMA";
	case TokenType::FILE_END: return "FILE_END";
	}
	return "UNRECOGNISED TOKEN";
}

struct Token {
	std::string contents;
	TokenType type;

	Token(std::string contents, TokenType type) : contents(contents), type(type) {}
	std::string str() { return contents + " (" + tokenTypeString(type) + ")"; }
};