#include "parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), index(0U) {
}

Node* Parser::parse() {
	return parseProgram();
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
	else if (tk.type == TokenType::ID) {
		node = parseName(NodeName::Usage::LOAD_TO_STACK);
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

Node* Parser::parseName(NodeName::Usage usage) {
	Token& tk = token();
	eat(TokenType::ID);
	return new NodeName(tk.contents, usage);
}

Node* Parser::parseFunctionCall() {
	NodeFunctionCall* functioncall = new NodeFunctionCall;
	Node* name = parseName(NodeName::Usage::FUNCTION_CALL);
	functioncall->addChild(name);
	eat(TokenType::LPARENTH);
	while (true) {
		Node* expr = parseArithmeticExpression();
		functioncall->addChild(expr);
		if (token().type != TokenType::COMMA)
			break;
	}
	eat(TokenType::RPARENTH);
	eat(TokenType::SEMICOLON);
	return functioncall;
}

Node* Parser::parseAssign() {
	Node* name = parseName(NodeName::Usage::NONE);
	//static_cast<NodeName*>(name)->dataType = DataType::INT;
	eat(TokenType::ASSIGN);
	Node* expr = parseArithmeticExpression();
	Node* node = new Node(NodeType::ASSIGN);
	node->addChild(name);
	node->addChild(expr);
	eat(TokenType::SEMICOLON);
	return node;
}

Node* Parser::parseDeclareAssign() {
	eat(TokenType::INT);
	Node* name = parseName(NodeName::Usage::NONE);
	static_cast<NodeName*>(name)->dataType = DataType::INT;
	eat(TokenType::ASSIGN);
	Node* expr = parseArithmeticExpression();
	Node* node = new Node(NodeType::DECLARE_ASSIGN);
	node->addChild(name);
	node->addChild(expr);
	eat(TokenType::SEMICOLON);
	return node;
}

Node* Parser::parseStatement() {
	switch (token().type) {
	case TokenType::INT:
		return parseDeclareAssign();

	case TokenType::ID:
		if (peek().type == TokenType::LPARENTH)
			return parseFunctionCall();
		// else
		return parseAssign();
	}

	throw std::runtime_error("Invalid token to start statement with: " + token().str() + "!");
}

Node* Parser::parseProgram() {
	NodeProgram* program = new NodeProgram;
	while (token().type != TokenType::FILE_END) {
		program->addChild(parseStatement());
	}
	return program;
}
