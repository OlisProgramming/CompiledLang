#include "obfuscator.h"

void obfuscateNames(Node* program, std::string symbolFname) {
	// In each scope, obfuscate names.
	// Each name gets an obfuscated value of the index it appears in the list of local variables.
	// For example, the first local variable declared will receive a value of 0,
	// and the second an obfuscated name of 1.

	std::ofstream symbolFile(symbolFname);
	std::unordered_map<std::string, int> map;

	int index = 0;
	for (Node* node : program->children) {
		if (node->type == NodeType::DECLARE_ASSIGN) {
			static_cast<NodeName*>(node->children[0])->obfuscatedName = index;  // Name
			map.emplace(static_cast<NodeName*>(node->children[0])->name, index);
			//std::cout << index << ": " << static_cast<NodeName*>(node->children[0])->name << std::endl;
			symbolFile << index++ << ": " << static_cast<NodeName*>(node->children[0])->name << std::endl;
		}
	}

	symbolFile.close();

	program->print();
	std::cout << std::endl << std::endl << std::endl;

	/*for (auto& pair : map) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}*/

	checkUnObfuscated(program, map);
}

void checkUnObfuscated(Node* node, std::unordered_map<std::string, int>& map) {
	for (Node* child : node->children) {
		checkUnObfuscated(child, map);
		if (child->type == NodeType::NAME) {
			NodeName* childName = static_cast<NodeName*>(child);
			if (childName->obfuscatedName == -1) {
				if (map.find(childName->name) != map.end()) {
					childName->obfuscatedName = map.find(childName->name)->second;
				}
				else {
					throw std::runtime_error("Variable " + childName->name + " was not obfuscated!");
				}
			}
		}
	}
}
