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
		return new NodeNumber(tk.contents, tk.pos, DataType(DataTypePrimitive::INTEGER, ""));
	case (TokenType::DOUBLE):
		eat(TokenType::DOUBLE);
		return new NodeNumber(tk.contents, tk.pos, DataType(DataTypePrimitive::DOUBLE, ""));
	case (TokenType::FLOAT):
		eat(TokenType::FLOAT);
		return new NodeNumber(tk.contents, tk.pos, DataType(DataTypePrimitive::FLOAT, ""));
	case (TokenType::BOOL):
		eat(TokenType::BOOL);
		return new NodeNumber(tk.contents, tk.pos, DataType(DataTypePrimitive::BOOL, ""));
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

Node* Parser::parseUnaryOperators1() {
	Token& tk = token();
	Node* node = parseArithmeticUnit();

	while (token().type == TokenType::CAST) {
		eat(TokenType::CAST);
		NodeCast* node2 = new NodeCast(tk.pos);
		node2->addChild(node);
		node2->typeToCast = token().contents;
		node = node2;
		switch (token().type) {

		case TokenType::KWD_INT:
			eat(TokenType::KWD_INT);
			break;
		case TokenType::KWD_DOUBLE:
			eat(TokenType::KWD_DOUBLE);
			break;
		case TokenType::KWD_FLOAT:
			eat(TokenType::KWD_FLOAT);
			break;
		default:
			eat(TokenType::KWD_BOOL);
		}
	}

	return node;
}

Node* Parser::parseUnaryOperators2() {
	Token& tk = token();
	Node* node;
	if (tk.type == TokenType::LOGIC_NOT) {
		eat(TokenType::LOGIC_NOT);
		node = new Node(NodeType::LOGIC_NOT, tk.pos);
		node->addChild(parseUnaryOperators2());
	}
	else {
		node = parseUnaryOperators1();
	}
	return node;
}

Node* Parser::parseMulDiv() {
	Token& tk = token();
	Node* node = parseUnaryOperators2();
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
		newNode->addChild(parseUnaryOperators2());
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

Node* Parser::parseComparison() {
	Token& tk = token();
	Node* node = parseAddSub();
	while (token().type == TokenType::GT || token().type == TokenType::LT
		|| token().type == TokenType::GE || token().type == TokenType::LE) {
		Node* newNode;
		if (token().type == TokenType::GT) {
			newNode = new Node(NodeType::GT, tk.pos);
			eat(TokenType::GT);
		}
		else if (token().type == TokenType::LT) {
			newNode = new Node(NodeType::LT, tk.pos);
			eat(TokenType::LT);
		}
		else if (token().type == TokenType::GE) {
			newNode = new Node(NodeType::GE, tk.pos);
			eat(TokenType::GE);
		}
		else {
			newNode = new Node(NodeType::LE, tk.pos);
			eat(TokenType::LE);
		}
		newNode->addChild(node);
		newNode->addChild(parseAddSub());
		node = newNode;
	}
	return node;
}

Node* Parser::parseEquality() {
	Token& tk = token();
	Node* node = parseComparison();
	while (token().type == TokenType::EQ || token().type == TokenType::NE) {
		Node* newNode;
		if (token().type == TokenType::EQ) {
			newNode = new Node(NodeType::EQ, tk.pos);
			eat(TokenType::EQ);
		}
		else {
			newNode = new Node(NodeType::NE, tk.pos);
			eat(TokenType::NE);
		}
		newNode->addChild(node);
		newNode->addChild(parseComparison());
		node = newNode;
	}
	return node;
}

Node* Parser::parseArithmeticExpression() {
	return parseEquality();
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
	//static_cast<NodeName*>(name)->dataType = DataTypePrimitive::INT;
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
		type.primitiveType = DataTypePrimitive::INTEGER;
		break;
	case TokenType::KWD_DOUBLE:
		eat(TokenType::KWD_DOUBLE);
		type.primitiveType = DataTypePrimitive::DOUBLE;
		break;
	case TokenType::KWD_FLOAT:
		eat(TokenType::KWD_FLOAT);
		type.primitiveType = DataTypePrimitive::FLOAT;
		break;
	case TokenType::KWD_BOOL:
		eat(TokenType::KWD_BOOL);
		type.primitiveType = DataTypePrimitive::BOOL;
		break;
	default:
		type.primitiveType = DataTypePrimitive::POINTER;
		type.typeName = token().contents;
		eat(TokenType::ID);
		break;
	}

	Node* name = parseName(NodeName::Usage::NONE);
	static_cast<NodeName*>(name)->dataType = type;
	
	Node* node;
	if (token().type == TokenType::ASSIGN) {
		eat(TokenType::ASSIGN);
		Node* expr = parseArithmeticExpression();
		node = new Node(NodeType::DECLARE_ASSIGN, tk.pos);
		node->addChild(name);
		node->addChild(expr);
	}
	else {
		node = new Node(NodeType::DECLARE, tk.pos);
		node->addChild(name);
	}

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
		else if (peek().type == TokenType::ID) {
			return parseDeclareAssign();
		}
		else {
			return parseAssign();
		}
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
