#include "typechecker.h"

void checkTypes(Node* node, std::unordered_map<std::string, DataType>& varTypes, std::unordered_map<std::string, FunctionPointer>& dependencies) {
	if (node->dataType != DataType::UNKNOWN) return;

	//std::cout << nodeTypeString(node->type) << std::endl;

	switch (node->type) {

	case NodeType::PROGRAM:
		node->dataType = DataType::INT;  // Could change later
		for (Node* child : node->children)
			checkTypes(child, varTypes, dependencies);
		break;

	case NodeType::DECLARE_ASSIGN:
		checkTypes(node->children[1], varTypes, dependencies);
		if (node->children[0]->dataType != node->children[1]->dataType) {
			throw std::runtime_error(
				"Cannot assign value of type "
				+ dataTypeString(node->children[1]->dataType) + " to variable of type "
				+ dataTypeString(node->children[0]->dataType) + "!");
		}
		node->dataType = node->children[0]->dataType;
		varTypes.emplace(static_cast<NodeName*>(node->children[0])->name, node->dataType);
		break;

	case NodeType::ASSIGN:
		checkTypes(node->children[0], varTypes, dependencies);
		checkTypes(node->children[1], varTypes, dependencies);
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
		checkTypes(node->children[0], varTypes, dependencies);
		checkTypes(node->children[1], varTypes, dependencies);
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
		if (static_cast<NodeName*>(node)->usage == NodeName::Usage::FUNCTION_CALL) {
			if (dependencies.find(static_cast<NodeName*>(node)->name) == dependencies.end()) {
				throw std::runtime_error("Function " + static_cast<NodeName*>(node)->name + " does not exist!");
			}
			node->dataType = dependencies.find(static_cast<NodeName*>(node)->name)->second.signature.returnType;
		}
		else {
			if (varTypes.find(static_cast<NodeName*>(node)->name) == varTypes.end()) {
				throw std::runtime_error("Variable " + static_cast<NodeName*>(node)->name + " does not exist yet!");
			}
			node->dataType = varTypes.find(static_cast<NodeName*>(node)->name)->second;
		}
		break;

	case NodeType::FUNCTION_CALL:
		//NodeFunctionCall* functioncall = static_cast<NodeFunctionCall*>(node);
		for (auto child : node->children)
			checkTypes(child, varTypes, dependencies);
		node->dataType = node->children[0]->dataType;
		static_cast<NodeFunctionCall*>(node)->pointer = dependencies.find(static_cast<NodeName*>(node->children[0])->name)->second;
		break;

	default:
		throw std::runtime_error("Could not check the data type of node {" + node->str() + "}!");
	}
}
