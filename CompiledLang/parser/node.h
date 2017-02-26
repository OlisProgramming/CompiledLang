#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "../tokeniser/token.h"

enum class NodeType {
	NUMBER,
	NAME,
	ADD, SUB, MUL, DIV,
	GT, LT, GE, LE, EQ, NE, LOGIC_NOT,
	ASSIGN, DECLARE_ASSIGN, DECLARE,
	FUNCTION_CALL,
	PROGRAM, NATIVE,
	CAST,
};

enum class DataTypePrimitive {
	UNKNOWN,
	VOID,
	INTEGER, DOUBLE, FLOAT, BOOL,
	POINTER
};

inline std::string nodeTypeString(NodeType& type) {
	switch (type) {
	case NodeType::NUMBER:			return "NUMBER";
	case NodeType::NAME:			return "NAME";
	case NodeType::ADD:				return "ADD";
	case NodeType::SUB:				return "SUB";
	case NodeType::MUL:				return "MUL";
	case NodeType::DIV:				return "DIV";
	case NodeType::GT:				return "GT";
	case NodeType::LT:				return "LT";
	case NodeType::GE:				return "GE";
	case NodeType::LE:				return "LE";
	case NodeType::EQ:				return "EQ";
	case NodeType::NE:				return "NE";
	case NodeType::LOGIC_NOT:		return "LOGIC_NOT";
	case NodeType::ASSIGN:			return "ASSIGN";
	case NodeType::DECLARE_ASSIGN:	return "DECLARE_ASSIGN";
	case NodeType::DECLARE:			return "DECLARE";
	case NodeType::FUNCTION_CALL:	return "FUNCTION_CALL";
	case NodeType::PROGRAM:			return "PROGRAM";
	case NodeType::NATIVE:			return "NATIVE";
	case NodeType::CAST:			return "CAST";
	}
	return "UNRECOGNISED NODE";
}

inline std::string dataTypeString(DataTypePrimitive& type) {
	switch (type) {
	case DataTypePrimitive::UNKNOWN:	return "unknown type";
	case DataTypePrimitive::VOID:		return "void";
	case DataTypePrimitive::INTEGER:	return "int";
	case DataTypePrimitive::DOUBLE:		return "double";
	case DataTypePrimitive::FLOAT:		return "float";
	case DataTypePrimitive::BOOL:		return "bool";
	case DataTypePrimitive::POINTER:	return "pointer";
	}
	return "UNRECOGNISED TYPE";
}

struct DataType {
	DataTypePrimitive primitiveType;
	std::string typeName;

	DataType() : primitiveType(DataTypePrimitive::UNKNOWN), typeName("") {}
	DataType(DataTypePrimitive primitiveType) : primitiveType(primitiveType), typeName("") {}
	DataType(std::string typeName) : primitiveType(DataTypePrimitive::POINTER), typeName(typeName) {}
	DataType(DataTypePrimitive primitiveType, std::string typeName) : primitiveType(primitiveType), typeName(typeName) {}
	std::string str() { return dataTypeString(primitiveType) + " named \"" + typeName + "\""; }
	bool operator ==(const DataType& other) const {
		if (primitiveType != DataTypePrimitive::POINTER)
			return (primitiveType == other.primitiveType);
		return (typeName == other.typeName);
	}
	bool operator !=(const DataType& other) const {
		if (primitiveType != DataTypePrimitive::POINTER)
			return (primitiveType != other.primitiveType);
		return (typeName != other.typeName);
	}
};

inline std::string dataTypeInitial(DataTypePrimitive& type) {
	switch (type) {
	case DataTypePrimitive::UNKNOWN:	return "?";
	case DataTypePrimitive::VOID:		return "v";
	case DataTypePrimitive::INTEGER:	return "i";
	case DataTypePrimitive::DOUBLE:		return "d";
	case DataTypePrimitive::FLOAT:		return "f";
	case DataTypePrimitive::BOOL:		return "b";
	case DataTypePrimitive::POINTER:	return "p";
	}
	return "?";
}

class Node {

public:
	NodeType type;
	DataType dataType;
	FilePos pos;
	std::vector<Node*> children;

	Node(NodeType type, FilePos pos, DataType dataType = DataType()) : type(type), pos(pos), dataType(dataType) {}
	~Node() { for (Node* child : children) delete child; }
	void addChild(Node* node) { children.push_back(node); }
	virtual std::string str() { return "<" + nodeTypeString(type) + " (" + dataType.str() + ")>"; }
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
	std::string str() override { return num + " (" + dataType.str() + ")"; }
};

class NodeCast : public Node {

public:
	std::string typeToCast;

	NodeCast(FilePos pos) : Node(NodeType::CAST, pos), typeToCast("") {}
	std::string str() override { return "cast to " + typeToCast + " (" + dataType.str() + ")"; }
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
	std::string str() override { return name + ": " + std::to_string(obfuscatedName) + " (" + dataType.str() + ")"; }
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

	FunctionSignature() : returnType(DataTypePrimitive::UNKNOWN, "") {}
	FunctionSignature(DataType returnType, std::vector<DataType> argTypes) : returnType(returnType), argTypes(argTypes) {}

	std::string str() {
		std::string args;
		for (unsigned int i = 0; i < argTypes.size(); ++i)
			args += argTypes[i].str() + ", ";
		args += "return " + returnType.str();
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
