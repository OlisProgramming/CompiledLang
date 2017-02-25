#include "dependencyresolver.h"

std::unordered_map<std::string, FunctionPointer> resolveDependencies() {
	std::unordered_map<std::string, FunctionPointer> dependencies;
	
	dependencies.emplace("println", FunctionPointer(DataType::INT, { DataType::INT }, new NodeNative("println")));
	
	return dependencies;
}