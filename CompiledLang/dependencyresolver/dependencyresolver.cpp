#include "dependencyresolver.h"

std::unordered_multimap<std::string, FunctionPointer> resolveDependencies() {
	std::unordered_multimap<std::string, FunctionPointer> dependencies;
	
	dependencies.emplace("println", FunctionPointer("iprintln", DataType::INTEGER, { DataType::INTEGER }, new NodeNative("iprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("dprintln", DataType::INTEGER, { DataType::DOUBLE }, new NodeNative("dprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("fprintln", DataType::INTEGER, { DataType::FLOAT }, new NodeNative("fprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("bprintln", DataType::INTEGER, { DataType::BOOL }, new NodeNative("bprintln", FilePos(0U, 0U, ""))));
	
	return dependencies;
}