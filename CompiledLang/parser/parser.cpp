#include "parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), index(0U) {
}

Node* Parser::parse() {
	return parseAddSub();
}

Node* Parser::parseNumber() {
	Token& tk = token();
	eat(TokenType::NUMBER);
	return new NodeNumber(tk.contents);
}

Node* Parser::parseArithmeticUnit() {
	Token& tk = token();
	Node* node;
	if (tk.type == TokenType::LPARENTH) {
		eat(TokenType::LPARENTH);
		node = parseArithmeticExpression();
		eat(TokenType::RPARENTH);
	}
	else
		node = parseNumber();
	return node;
}

Node* Parser::parseMulDiv() {
	Node* node = parseArithmeticUnit();
	while (token().type == TokenType::MUL || token().type == TokenType::DIV) {
		Node* newNode;
		if (token().type == TokenType::MUL) {
			newNode = new Node(NodeType::MUL);
			eat(TokenType::MUL);
		}
		else {
			newNode = new Node(NodeType::DIV);
			eat(TokenType::DIV);
		}
		newNode->addChild(node);
		newNode->addChild(parseArithmeticUnit());
		node = newNode;
	}
	return node;
}

Node* Parser::parseAddSub() {
	Node* node = parseMulDiv();
	while (token().type == TokenType::ADD || token().type == TokenType::SUB) {
		Node* newNode;
		if (token().type == TokenType::ADD) {
			newNode = new Node(NodeType::ADD);
			eat(TokenType::ADD);
		}
		else {
			newNode = new Node(NodeType::SUB);
			eat(TokenType::SUB);
		}
		newNode->addChild(node);
		newNode->addChild(parseMulDiv());
		node = newNode;
	}
	return node;
}

Node* Parser::parseArithmeticExpression() {
	return parseAddSub();
}
