#include "translator.h"

std::string Translator::translate(std::string symbolFname) {
	output = "";

	std::ofstream symbolFile(symbolFname, std::ios_base::app);
	symbolFile << std::endl << "FUNCTIONS (name: line)" << std::endl;

	send("");  // Create a blank line at start for initial goto statement

	// Add dependencies
	for (auto dependencyPair : dependencies) {
		dependencyLineNumbers.emplace(dependencyPair.second.nativeName, currentLine);
		symbolFile << dependencyPair.first << " (" << dependencyPair.second.signature.str() << "): " << currentLine << std::endl;
		translate(dependencyPair.second.program);
		if (dependencyPair.second.signature.returnType == DataType::VOID)
			send("return");
		else
			send("sreturn");
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
	case NodeType::NUMBER:  // Load constant
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

	case NodeType::CAST: {
		std::string to = dataTypeInitial(node->dataType);
		std::string from = dataTypeInitial(node->children[0]->dataType);
		send(from + "2" + to);
		break;
	}

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

	case NodeType::GT:
		if (node->children[0]->dataType == DataType::INTEGER)
			send("igt");
		else if (node->children[0]->dataType == DataType::DOUBLE)
			send("dgt");
		else if (node->children[0]->dataType == DataType::FLOAT)
			send("fgt");
		else
			throw std::runtime_error("Cannot interpret GT node of type " + dataTypeString(node->children[0]->dataType) + "!");
		break;

	case NodeType::LT:
		if (node->children[0]->dataType == DataType::INTEGER)
			send("ilt");
		else if (node->children[0]->dataType == DataType::DOUBLE)
			send("dlt");
		else if (node->children[0]->dataType == DataType::FLOAT)
			send("flt");
		else
			throw std::runtime_error("Cannot interpret LT node of type " + dataTypeString(node->children[0]->dataType) + "!");
		break;

	case NodeType::GE:
		if (node->children[0]->dataType == DataType::INTEGER)
			send("ige");
		else if (node->children[0]->dataType == DataType::DOUBLE)
			send("dge");
		else if (node->children[0]->dataType == DataType::FLOAT)
			send("fge");
		else
			throw std::runtime_error("Cannot interpret GE node of type " + dataTypeString(node->children[0]->dataType) + "!");
		break;

	case NodeType::LE:
		if (node->children[0]->dataType == DataType::INTEGER)
			send("ile");
		else if (node->children[0]->dataType == DataType::DOUBLE)
			send("dle");
		else if (node->children[0]->dataType == DataType::FLOAT)
			send("fle");
		else
			throw std::runtime_error("Cannot interpret LE node of type " + dataTypeString(node->children[0]->dataType) + "!");
		break;

	case NodeType::EQ:
		if (node->children[0]->dataType == DataType::INTEGER)
			send("ieq");
		else if (node->children[0]->dataType == DataType::DOUBLE)
			send("deq");
		else if (node->children[0]->dataType == DataType::FLOAT)
			send("feq");
		else
			throw std::runtime_error("Cannot interpret EQ node of type " + dataTypeString(node->children[0]->dataType) + "!");
		break;

	case NodeType::NE:
		if (node->dataType == DataType::INTEGER)
			send("ine");
		else if (node->dataType == DataType::DOUBLE)
			send("dne");
		else if (node->dataType == DataType::FLOAT)
			send("fne");
		else
			throw std::runtime_error("Cannot interpret NE node of type " + dataTypeString(node->children[0]->dataType) + "!");
		break;

	case NodeType::LOGIC_NOT:
		send("lognot");
		break;

	case NodeType::NAME:
		if (static_cast<NodeName*>(node)->usage == NodeName::Usage::LOAD_TO_STACK) {
			send("iload " + std::to_string(static_cast<NodeName*>(node)->obfuscatedName));
		}
		break;

	case NodeType::DECLARE_ASSIGN:
	case NodeType::ASSIGN:
		send(dataTypeInitial(static_cast<NodeName*>(node->children[0])->dataType) + "store " + std::to_string(static_cast<NodeName*>(node->children[0])->obfuscatedName));
		break;

	case NodeType::PROGRAM:
		// Children handled above
		break;

	case NodeType::FUNCTION_CALL:
		send("goto " + std::to_string(dependencyLineNumbers[static_cast<NodeFunctionCall*>(node)->pointer.nativeName]));
		break;

	case NodeType::NATIVE:
		send(static_cast<NodeNative*>(node)->code);
		break;

	default:
		throw std::runtime_error("Invalid node type for translator '" + nodeTypeString(node->type) + "'!");
	}
}
