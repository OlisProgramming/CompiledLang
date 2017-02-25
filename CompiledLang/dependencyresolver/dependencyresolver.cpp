#include "dependencyresolver.h"

std::unordered_multimap<std::string, FunctionPointer> resolveDependencies() {
	std::unordered_multimap<std::string, FunctionPointer> dependencies;
	
	dependencies.emplace("println", FunctionPointer("iprintln", DataType::VOID, { DataType::INTEGER }, new NodeNative("iprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("dprintln", DataType::VOID, { DataType::DOUBLE }, new NodeNative("dprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("fprintln", DataType::VOID, { DataType::FLOAT }, new NodeNative("fprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("bprintln", DataType::VOID, { DataType::BOOL }, new NodeNative("bprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("gendouble", FunctionPointer("gendouble", DataType::DOUBLE, { }, new NodeNative("dldc 3.14159265", FilePos(0U, 0U, ""))));
	
	return dependencies;
}