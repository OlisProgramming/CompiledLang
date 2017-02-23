#include "translator.h"

void Translator::translate(Node* node) {

	for (Node* child : node->children) translate(child);

	switch (node->type) {
	case NodeType::NUMBER:
		send("ldc " + dynamic_cast<NodeNumber*>(node)->num);
		return;

	case NodeType::ADD:
		send("iadd");
		return;

	case NodeType::SUB:
		send("isub");
		return;

	case NodeType::MUL:
		send("imul");
		return;

	case NodeType::DIV:
		send("idiv");
		return;

	default:
		throw std::runtime_error("Invalid node type for translator '" + nodeTypeString(node->type) + "'!");
	}
}
