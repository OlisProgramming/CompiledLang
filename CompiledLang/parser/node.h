#pragma once

#include <string>
#include <vector>
#include <iostream>

enum class NodeType {
	NUMBER,
	NAME,
	ADD, SUB, MUL, DIV,
	ASSIGN, DECLARE_ASSIGN,
	FUNCTION_CALL,
	PROGRAM, NATIVE
};

enum class DataType {
	UNKNOWN,
	INT
};

inline std::string nodeTypeString(NodeType& type) {
	switch (type) {
	case NodeType::NUMBER: return "NUMBER";
	case NodeType::NAME: return "NAME";
	case NodeType::ADD: return "ADD";
	case NodeType::SUB: return "SUB";
	case NodeType::MUL: return "MUL";
	case NodeType::DIV: return "DIV";
	case NodeType::ASSIGN: return "ASSIGN";
	case NodeType::DECLARE_ASSIGN: return "DECLARE_ASSIGN";
	case NodeType::FUNCTION_CALL: return "FUNCTION_CALL";
	case NodeType::PROGRAM: return "PROGRAM";
	case NodeType::NATIVE: return "NATIVE";
	}
	return "UNRECOGNISED NODE";
}

inline std::string dataTypeString(DataType& type) {
	switch (type) {
	case DataType::UNKNOWN: return "unknown type";
	case DataType::INT: return "int";
	}
	return "UNRECOGNISED TYPE";
}

class Node {

public:
	NodeType type;
	DataType dataType;
	std::vector<Node*> children;

	Node(NodeType type, DataType dataType = DataType::UNKNOWN) : type(type), dataType(dataType) {}
	~Node() { for (Node* child : children) delete child; }
	void addChild(Node* node) { children.push_back(node); }
	virtual std::string str() { return "<" + nodeTypeString(type) + " (" + dataTypeString(dataType) + ")>"; }
	void print(unsigned int indent = 0) {
		std::cout << std::string(indent, ' ') << str() << '\n';
		for (auto child : children)
			child->print(indent + 2);
	}
};

class NodeNumber : public Node {

public:
	std::string num;

	NodeNumber(std::string num) : Node(NodeType::NUMBER, DataType::INT), num(num) {}
	std::string str() override { return num + " (" + dataTypeString(dataType) + ")"; }
};

class NodeName : public Node {

public:
	std::string name;
	enum class Usage {
		NONE,
		LOAD_TO_STACK,
		FUNCTION_CALL
	} usage;
	int obfuscatedName;

	NodeName(std::string name, Usage usage) : Node(NodeType::NAME), name(name), obfuscatedName(-1), usage(usage) {}
	std::string str() override { return name + ": " + std::to_string(obfuscatedName) + " (" + dataTypeString(dataType) + ")"; }
};

class NodeProgram : public Node {

public:
	int maxLocalVars;

	NodeProgram() : Node(NodeType::PROGRAM), maxLocalVars(0) {}
};

class NodeNative : public Node {

public:
	std::string code;

	NodeNative(std::string code) : Node(NodeType::NATIVE), code(code) {}
};

struct FunctionSignature {
	DataType returnType;
	std::vector<DataType> argTypes;

	FunctionSignature() {}
	FunctionSignature(DataType returnType, std::vector<DataType> argTypes) : returnType(returnType), argTypes(argTypes) {}

	std::string str() {
		std::string args;
		for (unsigned int i = 0; i < argTypes.size(); ++i)
			args += dataTypeString(argTypes[i]) + ", ";
		args += "return " + dataTypeString(returnType);
		return args;
	}
};

struct FunctionPointer {
	FunctionSignature signature;
	Node* program;

	FunctionPointer() {}
	FunctionPointer(DataType returnType, std::vector<DataType> argTypes, Node* program) : signature(returnType, argTypes), program(program) {}
	std::string str() { return signature.str() + ((program==nullptr)? " (function node not set)" : " (valid function)"); }
};

class NodeFunctionCall : public Node {

public:
	FunctionPointer pointer;

	NodeFunctionCall() : Node(NodeType::FUNCTION_CALL) {}
	std::string str() override {
		return "<FUNCTION CALL " + pointer.str() + ">";
	}
};
