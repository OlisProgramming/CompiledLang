#include "typechecker.h"

void checkTypes(Node* node) {
	if (node->dataType != DataType::UNKNOWN) return;

	//std::cout << nodeTypeString(node->type) << std::endl;

	switch (node->type) {

	case NodeType::PROGRAM:
		node->dataType = DataType::INT;  // Could change later
		for (Node* child : node->children)
			checkTypes(child);
		break;

	case NodeType::DECLARE_ASSIGN:
		checkTypes(node->children[1]);
		if (node->children[0]->dataType != node->children[1]->dataType) {
			throw std::runtime_error(
				"Cannot assign value of type "
				+ dataTypeString(node->children[1]->dataType) + " to variable of type "
				+ dataTypeString(node->children[0]->dataType) + "!");
		}
		node->dataType = node->children[0]->dataType;
		break;

	case NodeType::ADD:
	case NodeType::SUB:
	case NodeType::MUL:
	case NodeType::DIV:
		checkTypes(node->children[0]);
		checkTypes(node->children[1]);
		if (node->children[0]->dataType != node->children[1]->dataType) {
			throw std::runtime_error(
				"Cannot apply operation " + nodeTypeString(node->type)
				+ " to children of different types ("
				+ dataTypeString(node->children[0]->dataType) + " and "
				+ dataTypeString(node->children[1]->dataType) + ")!");
		}
		node->dataType = node->children[0]->dataType;
		break;

	default:
		throw std::runtime_error("Could not check the data type of node " + nodeTypeString(node->type) + "!");
	}
}
