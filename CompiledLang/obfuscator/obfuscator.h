#pragma once

#include <fstream>
#include <unordered_map>
#include "../parser/node.h"

void obfuscateNames(Node* program, std::string symbolFname);
void checkUnObfuscated(Node* node, std::unordered_map<std::string, int>& map);
