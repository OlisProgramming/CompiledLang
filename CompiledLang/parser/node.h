#pragma once

#include <string>
#include <vector>
#include <iostream>

enum class NodeType {
	NUMBER,
	ADD, SUB, MUL, DIV
};

inline std::string nodeTypeString(NodeType& type) {
	switch (type) {
	case NodeType::NUMBER: return "NUMBER";
	case NodeType::ADD: return "ADD";
	case NodeType::SUB: return "SUB";
	case NodeType::MUL: return "MUL";
	case NodeType::DIV: return "DIV";
	}
	return "UNRECOGNISED NODE";
}

class Node {

public:
	NodeType type;
	std::vector<Node*> children;

	Node(NodeType type) : type(type) {}
	~Node() { for (Node* child : children) delete child; }
	void addChild(Node* node) { children.push_back(node); }
	virtual std::string str() { return "<" + nodeTypeString(type) + ">"; }
	void print(unsigned int indent = 0) {
		std::cout << std::string(indent, ' ') << str() << '\n';
		for (auto child : children)
			child->print(indent + 2);
	}
};

class NodeNumber : public Node {

public:
	std::string num;

	NodeNumber(std::string num) : Node(NodeType::NUMBER), num(num) {}
	std::string str() override { return num; }
};