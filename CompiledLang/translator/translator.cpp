#include "translator.h"

std::string Translator::translate(std::string symbolFname) {
	output = "";

	std::ofstream symbolFile(symbolFname, std::ios_base::app);
	symbolFile << std::endl << "FUNCTIONS (name: line)" << std::endl;

	send("");  // Create a blank line at start for initial goto statement

	// Add dependencies
	for (auto dependencyPair : dependencies) {
		dependencyLineNumbers.emplace(dependencyPair.first, currentLine);
		symbolFile << dependencyPair.first << " (" << dependencyPair.second.signature.str() << "): " << currentLine << std::endl;
		translate(dependencyPair.second.program);
		send("ireturn 0");
	}

	output = "goto " + std::to_string(currentLine) + output;
	symbolFile << "main program: " << currentLine << std::endl;

	translate(program);
	return output;
}

void Translator::translate(Node* node) {

	if (node->type == NodeType::PROGRAM) {
		send("frame_alloc " + std::to_string(static_cast<NodeProgram*>(node)->maxLocalVars));  // Allocate room for local variables on stack.
	}

	for (Node* child : node->children) translate(child);

	switch (node->type) {
	case NodeType::NUMBER:
		send("ldc " + dynamic_cast<NodeNumber*>(node)->num);
		return;

	case NodeType::ADD:
		if (node->dataType == DataType::INT)
			send("iadd");
		else
			throw std::runtime_error("Cannot interpret ADD node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::SUB:
		if (node->dataType == DataType::INT)
			send("isub");
		else
			throw std::runtime_error("Cannot interpret SUB node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::MUL:
		if (node->dataType == DataType::INT)
			send("imul");
		else
			throw std::runtime_error("Cannot interpret MUL node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::DIV:
		if (node->dataType == DataType::INT)
			send("idiv");
		else
			throw std::runtime_error("Cannot interpret DIV node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::NAME:
		if (static_cast<NodeName*>(node)->usage == NodeName::Usage::LOAD_TO_STACK) {
			send("iload " + std::to_string(static_cast<NodeName*>(node)->obfuscatedName));
		}
		break;

	case NodeType::DECLARE_ASSIGN:
	case NodeType::ASSIGN:
		send("istore " + std::to_string(static_cast<NodeName*>(node->children[0])->obfuscatedName));
		break;

	case NodeType::PROGRAM:
		// Children handled above
		break;

	case NodeType::FUNCTION_CALL:
		send("goto " + std::to_string(dependencyLineNumbers[static_cast<NodeName*>(node->children[0])->name]));
		break;

	case NodeType::NATIVE:
		send("native " + static_cast<NodeNative*>(node)->code);
		break;

	default:
		throw std::runtime_error("Invalid node type for translator '" + nodeTypeString(node->type) + "'!");
	}
}
