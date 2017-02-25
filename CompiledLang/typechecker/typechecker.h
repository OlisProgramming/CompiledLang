#pragma once

#include "../parser/node.h"
#include <unordered_map>

void checkTypes(Node* program, std::unordered_map<std::string, DataType>& varTypes, std::unordered_multimap<std::string, FunctionPointer>& dependencies);
