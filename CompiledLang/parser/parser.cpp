#include "parser.h"

Parser::Parser(std::vector<Token> tokens) : tokens(tokens), index(0U) {
}

Node* Parser::parse() {
	return parseProgram();
}

Node* Parser::parseNumber() {
	Token& tk = token();
	switch (tk.type) {
	case (TokenType::INTEGER):
		eat(TokenType::INTEGER);
		return new NodeNumber(tk.contents, tk.pos, DataType::INTEGER);
	case (TokenType::DOUBLE):
		eat(TokenType::DOUBLE);
		return new NodeNumber(tk.contents, tk.pos, DataType::DOUBLE);
	case (TokenType::FLOAT):
		eat(TokenType::FLOAT);
		return new NodeNumber(tk.contents, tk.pos, DataType::FLOAT);
	case (TokenType::BOOL):
		eat(TokenType::BOOL);
		return new NodeNumber(tk.contents, tk.pos, DataType::BOOL);
	}
	eat(TokenType::INTEGER);
	return nullptr;  // Will never get here, always quits on 'eat' because it passed through the switch.
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
		if (peek().type == TokenType::LPARENTH)
			node = parseFunctionCall();
		else
			node = parseName(NodeName::Usage::LOAD_TO_STACK);
	}
	else
		node = parseNumber();
	return node;
}

Node* Parser::parseMulDiv() {
	Token& tk = token();
	Node* node = parseArithmeticUnit();
	while (token().type == TokenType::MUL || token().type == TokenType::DIV) {
		Node* newNode;
		if (token().type == TokenType::MUL) {
			newNode = new Node(NodeType::MUL, tk.pos);
			eat(TokenType::MUL);
		}
		else {
			newNode = new Node(NodeType::DIV, tk.pos);
			eat(TokenType::DIV);
		}
		newNode->addChild(node);
		newNode->addChild(parseArithmeticUnit());
		node = newNode;
	}
	return node;
}

Node* Parser::parseAddSub() {
	Token& tk = token();
	Node* node = parseMulDiv();
	while (token().type == TokenType::ADD || token().type == TokenType::SUB) {
		Node* newNode;
		if (token().type == TokenType::ADD) {
			newNode = new Node(NodeType::ADD, tk.pos);
			eat(TokenType::ADD);
		}
		else {
			newNode = new Node(NodeType::SUB, tk.pos);
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
	return new NodeName(tk.contents, tk.pos, usage);
}

Node* Parser::parseFunctionCall() {
	Token& tk = token();
	NodeFunctionCall* functioncall = new NodeFunctionCall(tk.pos);
	Node* name = parseName(NodeName::Usage::FUNCTION_CALL);
	functioncall->addChild(name);
	eat(TokenType::LPARENTH);
	if (token().type != TokenType::RPARENTH) {
		while (true) {
			Node* expr = parseArithmeticExpression();
			functioncall->addChild(expr);
			if (token().type != TokenType::COMMA)
				break;
		}
	}
	eat(TokenType::RPARENTH);
	return functioncall;
}

Node* Parser::parseAssign() {
	Token& tk = token();
	Node* name = parseName(NodeName::Usage::NONE);
	//static_cast<NodeName*>(name)->dataType = DataType::INT;
	eat(TokenType::ASSIGN);
	Node* expr = parseArithmeticExpression();
	Node* node = new Node(NodeType::ASSIGN, tk.pos);
	node->addChild(name);
	node->addChild(expr);
	eat(TokenType::SEMICOLON);
	return node;
}

Node* Parser::parseDeclareAssign() {
	Token& tk = token();
	DataType type;
	switch (token().type) {
	case TokenType::KWD_INT:
		eat(TokenType::KWD_INT);
		type = DataType::INTEGER;
		break;
	case TokenType::KWD_DOUBLE:
		eat(TokenType::KWD_DOUBLE);
		type = DataType::DOUBLE;
		break;
	case TokenType::KWD_FLOAT:
		eat(TokenType::KWD_FLOAT);
		type = DataType::FLOAT;
		break;
	default:
		eat(TokenType::KWD_BOOL);
		type = DataType::BOOL;
		break;
	}

	Node* name = parseName(NodeName::Usage::NONE);
	static_cast<NodeName*>(name)->dataType = type;
	eat(TokenType::ASSIGN);
	Node* expr = parseArithmeticExpression();
	Node* node = new Node(NodeType::DECLARE_ASSIGN, tk.pos);
	node->addChild(name);
	node->addChild(expr);
	eat(TokenType::SEMICOLON);
	return node;
}

Node* Parser::parseStatement() {
	switch (token().type) {
	case TokenType::KWD_INT:
	case TokenType::KWD_DOUBLE:
	case TokenType::KWD_FLOAT:
	case TokenType::KWD_BOOL:
		return parseDeclareAssign();

	case TokenType::ID:
		if (peek().type == TokenType::LPARENTH) {
			Node* node = parseFunctionCall();
			eat(TokenType::SEMICOLON);
			return node;
		}
		// else
		return parseAssign();
	}

	throw std::runtime_error("Invalid token to start statement with: " + token().str() + "!");
}

Node* Parser::parseProgram() {
	Token& tk = token();
	NodeProgram* program = new NodeProgram(tk.pos);
	while (token().type != TokenType::FILE_END) {
		program->addChild(parseStatement());
	}
	return program;
}
