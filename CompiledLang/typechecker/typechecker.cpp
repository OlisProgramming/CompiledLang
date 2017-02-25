#include "typechecker.h"

void checkTypes(Node* node, std::unordered_map<std::string, DataType>& varTypes, std::unordered_multimap<std::string, FunctionPointer>& dependencies) {
	if (node->dataType != DataType::UNKNOWN) return;

	//std::cout << nodeTypeString(node->type) << std::endl;

	switch (node->type) {

	case NodeType::PROGRAM:
		node->dataType = DataType::VOID;  // Could change later
		for (Node* child : node->children)
			checkTypes(child, varTypes, dependencies);
		break;

	case NodeType::DECLARE_ASSIGN:
		checkTypes(node->children[1], varTypes, dependencies);
		if (node->children[0]->dataType != node->children[1]->dataType) {
			throw std::runtime_error(
				"Cannot assign value of type "
				+ dataTypeString(node->children[1]->dataType) + " to variable of type "
				+ dataTypeString(node->children[0]->dataType) + "! Error at " + node->pos.str());
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
				+ dataTypeString(node->children[0]->dataType) + "! Error at " + node->pos.str());
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
				+ dataTypeString(node->children[1]->dataType) + ")! Error at " + node->pos.str());
		}
		node->dataType = node->children[0]->dataType;
		break;

	case NodeType::NAME:
		if (static_cast<NodeName*>(node)->usage == NodeName::Usage::FUNCTION_CALL) {
			if (dependencies.count(static_cast<NodeName*>(node)->name) == 0) {
				throw std::runtime_error("Function " + static_cast<NodeName*>(node)->name + " does not exist! Error at " + node->pos.str());
			}
			//node->dataType = dependencies.find(static_cast<NodeName*>(node)->name)->second.signature.returnType;
		}
		else {
			if (varTypes.find(static_cast<NodeName*>(node)->name) == varTypes.end()) {
				throw std::runtime_error("Variable " + static_cast<NodeName*>(node)->name + " does not exist yet! Error at " + node->pos.str());
			}
			node->dataType = varTypes.find(static_cast<NodeName*>(node)->name)->second;
		}
		break;

	case NodeType::FUNCTION_CALL: {
		//NodeFunctionCall* functioncall = static_cast<NodeFunctionCall*>(node);
		for (auto child : node->children)
			checkTypes(child, varTypes, dependencies);
		std::vector<DataType> argTypes;
		for (unsigned int i = 1; i < node->children.size(); i++) {
			argTypes.push_back(node->children[i]->dataType);
		}

		//std::cout << "Matching from: ";
		//for (auto arg : argTypes)
		//	std::cout << dataTypeString(arg);
		//std::cout << std::endl;

		bool successful = false;
		auto its = dependencies.equal_range(static_cast<NodeName*>(node->children[0])->name);
		for (auto pair = its.first; pair != its.second; ++pair) {

			//std::cout << "Matching to: ";
			//for (auto arg : pair->second.signature.argTypes)
			//	std::cout << dataTypeString(arg);
			//std::cout << std::endl;

			if (pair->second.signature.argTypes == argTypes) {
				node->dataType = pair->second.signature.returnType;

				//std::cout << "Matched function: " << pair->first << std::endl;

				static_cast<NodeFunctionCall*>(node)->pointer.signature = pair->second.signature;
				static_cast<NodeFunctionCall*>(node)->pointer.nativeName = pair->second.nativeName;
				node->children[0]->dataType = pair->second.signature.returnType;
				successful = true;
				break;
			}
		}
		if (!successful) {
			throw std::runtime_error("No instance of overloaded function " + static_cast<NodeName*>(node->children[0])->name + " matches argument types! Error at " + node->pos.str());
		}

		//static_cast<NodeFunctionCall*>(node)->pointer = dependencies.find(static_cast<NodeName*>(node->children[0])->name)->second;
		break;
	}

	default:
		throw std::runtime_error("Could not check the data type of node {" + node->str() + "}! Error at " + node->pos.str());
	}
}
