#include "dependencyresolver.h"

std::unordered_multimap<std::string, FunctionPointer> resolveDependencies() {
	std::unordered_multimap<std::string, FunctionPointer> dependencies;
	
	dependencies.emplace("println", FunctionPointer("iprintln", DataTypePrimitive::VOID, { DataTypePrimitive::INTEGER }, new NodeNative("iprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("dprintln", DataTypePrimitive::VOID, { DataTypePrimitive::DOUBLE }, new NodeNative("dprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("fprintln", DataTypePrimitive::VOID, { DataTypePrimitive::FLOAT }, new NodeNative("fprintln", FilePos(0U, 0U, ""))));
	dependencies.emplace("println", FunctionPointer("bprintln", DataTypePrimitive::VOID, { DataTypePrimitive::BOOL }, new NodeNative("bprintln", FilePos(0U, 0U, ""))));
	
	dependencies.emplace("String", FunctionPointer("String", { "String" }, { }, new NodeNative("new String", FilePos(0U, 0U, ""))));
	
	return dependencies;
}