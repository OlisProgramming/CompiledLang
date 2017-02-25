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
		if (node->dataType == DataType::INTEGER)
			send("ildc " + dynamic_cast<NodeNumber*>(node)->num);
		else if (node->dataType == DataType::DOUBLE)
			send("dldc " + dynamic_cast<NodeNumber*>(node)->num);
		else if (node->dataType == DataType::FLOAT)
			send("fldc " + dynamic_cast<NodeNumber*>(node)->num);
		else if (node->dataType == DataType::BOOL)
			send("bldc " + ((dynamic_cast<NodeNumber*>(node)->num == "true") ? std::string("1") : std::string("0")));
		else
			throw std::runtime_error("Cannot load constant of data type " + dataTypeString(node->dataType) + "! Error at " + node->pos.str());
		return;

	case NodeType::ADD:
		if (node->dataType == DataType::INTEGER)
			send("iadd");
		else if (node->dataType == DataType::DOUBLE)
			send("dadd");
		else if (node->dataType == DataType::FLOAT)
			send("fadd");
		else
			throw std::runtime_error("Cannot interpret ADD node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::SUB:
		if (node->dataType == DataType::INTEGER)
			send("isub");
		else if (node->dataType == DataType::DOUBLE)
			send("dsub");
		else if (node->dataType == DataType::FLOAT)
			send("fsub");
		else
			throw std::runtime_error("Cannot interpret SUB node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::MUL:
		if (node->dataType == DataType::INTEGER)
			send("imul");
		else if (node->dataType == DataType::DOUBLE)
			send("dmul");
		else if (node->dataType == DataType::FLOAT)
			send("fmul");
		else
			throw std::runtime_error("Cannot interpret MUL node of type " + dataTypeString(node->dataType) + "!");
		return;

	case NodeType::DIV:
		if (node->dataType == DataType::INTEGER)
			send("idiv");
		else if (node->dataType == DataType::DOUBLE)
			send("ddiv");
		else if (node->dataType == DataType::FLOAT)
			send("fdiv");
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
