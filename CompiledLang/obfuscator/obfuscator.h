#pragma once

#include <fstream>
#include <unordered_map>
#include "../parser/node.h"

void obfuscateNames(Node* program, std::string symbolFname, std::unordered_map<std::string, FunctionPointer>& dependencies);
void checkUnObfuscated(Node* node, std::unordered_map<std::string, int>& map, std::unordered_map<std::string, FunctionPointer>& dependencies);
