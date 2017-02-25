#include "dependencyresolver.h"

std::unordered_map<std::string, FunctionPointer> resolveDependencies() {
	std::unordered_map<std::string, FunctionPointer> dependencies;
	
	dependencies.emplace("println", FunctionPointer(DataType::INTEGER, { DataType::INTEGER }, new NodeNative("println", FilePos(0U, 0U, ""))));
	
	return dependencies;
}