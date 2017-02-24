#include "typechecker.h"

void checkTypes(Node* node, std::unordered_map<std::string, DataType>& varTypes) {
	if (node->dataType != DataType::UNKNOWN) return;

	//std::cout << nodeTypeString(node->type) << std::endl;

	switch (node->type) {

	case NodeType::PROGRAM:
		node->dataType = DataType::INT;  // Could change later
		for (Node* child : node->children)
			checkTypes(child, varTypes);
		break;

	case NodeType::DECLARE_ASSIGN:
		checkTypes(node->children[1], varTypes);
		if (node->children[0]->dataType != node->children[1]->dataType) {
			throw std::runtime_error(
				"Cannot assign value of type "
				+ dataTypeString(node->children[1]->dataType) + " to variable of type "
				+ dataTypeString(node->children[0]->dataType) + "!");
		}
		node->dataType = node->children[0]->dataType;
		varTypes.emplace(static_cast<NodeName*>(node->children[0])->name, node->dataType);
		break;

	case NodeType::ADD:
	case NodeType::SUB:
	case NodeType::MUL:
	case NodeType::DIV:
		checkTypes(node->children[0], varTypes);
		checkTypes(node->children[1], varTypes);
		if (node->children[0]->dataType != node->children[1]->dataType) {
			throw std::runtime_error(
				"Cannot apply operation " + nodeTypeString(node->type)
				+ " to children of different types ("
				+ dataTypeString(node->children[0]->dataType) + " and "
				+ dataTypeString(node->children[1]->dataType) + ")!");
		}
		node->dataType = node->children[0]->dataType;
		break;

	case NodeType::NAME:
		if (varTypes.find(static_cast<NodeName*>(node)->name) == varTypes.end()) {
			throw std::runtime_error("Variable " + static_cast<NodeName*>(node)->name + " does not exist yet!");
		}
		static_cast<NodeName*>(node)->dataType = varTypes.find(static_cast<NodeName*>(node)->name)->second;
		break;

	default:
		throw std::runtime_error("Could not check the data type of node {" + node->str() + "}!");
	}
}
