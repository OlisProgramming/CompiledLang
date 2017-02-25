#pragma once

#include <string>

enum class TokenType {
	INTEGER, FLOAT, DOUBLE, BOOL,
	ADD, SUB, MUL, DIV,
	LPARENTH, RPARENTH,
	ID, CAST,
	KWD_INT, KWD_DOUBLE, KWD_FLOAT, KWD_BOOL,
	ASSIGN,
	GT, LT, GE, LE, EQ, NE, LOGIC_NOT,
	SEMICOLON, COMMA,
	FILE_END
};

inline std::string tokenTypeString(TokenType type) {
	switch (type) {
	case TokenType::INTEGER:	return "INTEGER";
	case TokenType::FLOAT:		return "FLOAT";
	case TokenType::DOUBLE:		return "DOUBLE";
	case TokenType::BOOL:		return "BOOL";
	case TokenType::ADD:		return "ADD";
	case TokenType::SUB:		return "SUB";
	case TokenType::MUL:		return "MUL";
	case TokenType::DIV:		return "DIV";
	case TokenType::LPARENTH:	return "LPARENTH";
	case TokenType::RPARENTH:	return "RPARENTH";
	case TokenType::ID:			return "ID";
	case TokenType::CAST:		return "CAST";
	case TokenType::KWD_INT:	return "KWD_INT";
	case TokenType::KWD_DOUBLE:	return "KWD_DOUBLE";
	case TokenType::KWD_FLOAT:	return "KWD_FLOAT";
	case TokenType::KWD_BOOL:	return "KWD_BOOL";
	case TokenType::ASSIGN:		return "ASSIGN";
	case TokenType::GT:			return "GT";
	case TokenType::LT:			return "LT";
	case TokenType::GE:			return "GE";
	case TokenType::LE:			return "LE";
	case TokenType::EQ:			return "EQ";
	case TokenType::NE:			return "NE";
	case TokenType::LOGIC_NOT:	return "LOGIC_NOT";
	case TokenType::SEMICOLON:	return "SEMICOLON";
	case TokenType::COMMA:		return "COMMA";
	case TokenType::FILE_END:	return "FILE_END";
	}
	return "UNRECOGNISED TOKEN";
}

struct FilePos {
	unsigned int line;
	unsigned int column;
	std::string fname;
	FilePos(unsigned int line, unsigned int column, std::string fname) : line(line), column(column), fname(fname) {}
	std::string str() {	return "line " + std::to_string(line) + ", column " + std::to_string(column) + " of file " + fname; }
};

struct Token {
	std::string contents;
	TokenType type;
	FilePos pos;

	Token(std::string contents, TokenType type, FilePos pos) : contents(contents), type(type), pos(pos) {}
	std::string str() { return contents + " (" + tokenTypeString(type) + ") at " + pos.str(); }
};