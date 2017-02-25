#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "../tokeniser/token.h"

enum class NodeType {
	NUMBER,
	NAME,
	ADD, SUB, MUL, DIV,
	ASSIGN, DECLARE_ASSIGN,
	FUNCTION_CALL,
	PROGRAM, NATIVE,
	CAST,
};

enum class DataType {
	UNKNOWN,
	VOID,
	INTEGER, DOUBLE, FLOAT, BOOL
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
	case NodeType::CAST: return "CAST";
	}
	return "UNRECOGNISED NODE";
}

inline std::string dataTypeString(DataType& type) {
	switch (type) {
	case DataType::UNKNOWN: return "unknown type";
	case DataType::VOID: return "void";
	case DataType::INTEGER: return "int";
	case DataType::DOUBLE: return "double";
	case DataType::FLOAT: return "float";
	case DataType::BOOL: return "bool";
	}
	return "UNRECOGNISED TYPE";
}

inline std::string dataTypeInitial(DataType& type) {
	switch (type) {
	case DataType::UNKNOWN: return "?";
	case DataType::VOID: return "v";
	case DataType::INTEGER: return "i";
	case DataType::DOUBLE: return "d";
	case DataType::FLOAT: return "f";
	case DataType::BOOL: return "b";
	}
	return "?";
}

class Node {

public:
	NodeType type;
	DataType dataType;
	FilePos pos;
	std::vector<Node*> children;

	Node(NodeType type, FilePos pos, DataType dataType = DataType::UNKNOWN) : type(type), pos(pos), dataType(dataType) {}
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

	NodeNumber(std::string num, FilePos pos, DataType dataType) : Node(NodeType::NUMBER, pos, dataType), num(num) {}
	std::string str() override { return num + " (" + dataTypeString(dataType) + ")"; }
};

class NodeCast : public Node {

public:
	std::string typeToCast;

	NodeCast(FilePos pos) : Node(NodeType::CAST, pos), typeToCast("") {}
	std::string str() override { return "cast to " + typeToCast + " (" + dataTypeString(dataType) + ")"; }
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

	NodeName(std::string name, FilePos pos, Usage usage) : Node(NodeType::NAME, pos), name(name), obfuscatedName(-1), usage(usage) {}
	std::string str() override { return name + ": " + std::to_string(obfuscatedName) + " (" + dataTypeString(dataType) + ")"; }
};

class NodeProgram : public Node {

public:
	int maxLocalVars;

	NodeProgram(FilePos pos) : Node(NodeType::PROGRAM, pos), maxLocalVars(0) {}
};

class NodeNative : public Node {

public:
	std::string code;

	NodeNative(std::string code, FilePos pos) : Node(NodeType::NATIVE, pos), code(code) {}
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
	std::string nativeName;

	FunctionPointer() {}
	FunctionPointer(std::string nativeName, DataType returnType, std::vector<DataType> argTypes, Node* program) : nativeName(nativeName), signature(returnType, argTypes), program(program) {}
	std::string str() { return signature.str() + ((program==nullptr)? " (no function node, named " : " (function node, named ") + ((nativeName == "")? "<no name>" : nativeName)  + ")"; }
};

class NodeFunctionCall : public Node {

public:
	FunctionPointer pointer;

	NodeFunctionCall(FilePos pos) : Node(NodeType::FUNCTION_CALL, pos) {}
	std::string str() override {
		return "<FUNCTION CALL " + pointer.str() + ">";
	}
};
