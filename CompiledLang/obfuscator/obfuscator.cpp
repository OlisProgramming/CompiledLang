#include "obfuscator.h"

void obfuscateNames(Node* program, std::string symbolFname, std::unordered_map<std::string, FunctionPointer>& dependencies) {
	// In each scope, obfuscate names.
	// Each name gets an obfuscated value of the index it appears in the list of local variables.
	// For example, the first local variable declared will receive a value of 0,
	// and the second an obfuscated name of 1.

	std::ofstream symbolFile(symbolFname);
	symbolFile << "VARIABLES (obfuscated name: name)" << std::endl;
	std::unordered_map<std::string, int> map;

	int index = 0;
	for (Node* node : program->children) {
		if (node->type == NodeType::DECLARE_ASSIGN) {
			static_cast<NodeName*>(node->children[0])->obfuscatedName = index;  // Name
			map.emplace(static_cast<NodeName*>(node->children[0])->name, index);
			//std::cout << index << ": " << static_cast<NodeName*>(node->children[0])->name << std::endl;
			symbolFile << index++ << " (" << dataTypeString(node->children[0]->dataType) << "): " << static_cast<NodeName*>(node->children[0])->name << std::endl;
		}
	}

	symbolFile.close();

	program->print();
	std::cout << std::endl << std::endl << std::endl;

	static_cast<NodeProgram*>(program)->maxLocalVars = index;

	/*for (auto& pair : map) {
		std::cout << pair.first << ": " << pair.second << std::endl;
	}*/

	checkUnObfuscated(program, map, dependencies);
}

void checkUnObfuscated(Node* node, std::unordered_map<std::string, int>& map, std::unordered_map<std::string, FunctionPointer>& dependencies) {
	for (Node* child : node->children) {
		checkUnObfuscated(child, map, dependencies);
		if (child->type == NodeType::NAME) {
			NodeName* childName = static_cast<NodeName*>(child);
			if (childName->obfuscatedName == -1) {
				if (map.find(childName->name) != map.end()) {
					childName->obfuscatedName = map.find(childName->name)->second;
				}
				else if (dependencies.find(childName->name) != dependencies.end()) {
					childName->obfuscatedName = -2;  // Obfuscated name for functions.
				}
				else {
					throw std::runtime_error("Variable " + childName->name + " was not obfuscated!");
				}
			}
		}
	}
}
