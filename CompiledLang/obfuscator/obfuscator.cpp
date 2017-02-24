#include "obfuscator.h"

void obfuscateNames(Node* program, std::string symbolFname) {
	// In each scope, obfuscate names.
	// Each name gets an obfuscated value of the index it appears in the list of local variables.
	// For example, the first local variable declared will receive a value of 0,
	// and the second an obfuscated name of 1.

	std::ofstream symbolFile(symbolFname);

	int index = 0;
	for (Node* node : program->children) {
		if (node->type == NodeType::DECLARE_ASSIGN) {
			static_cast<NodeName*>(node->children[0])->obfuscatedName = index;  // Name
			symbolFile << index++ << ": " << static_cast<NodeName*>(node->children[0])->name << std::endl;
		}
	}

	checkUnObfuscated(program);

	symbolFile.close();
}

void checkUnObfuscated(Node* node) {
	for (Node* child : node->children) {
		checkUnObfuscated(child);
		if (child->type == NodeType::NAME) {
			NodeName* childName = static_cast<NodeName*>(child);
			if (childName->obfuscatedName == -1) {
				throw std::runtime_error("Variable " + childName->name + " was not obfuscated!");
			}
		}
	}
}
