#pragma once

#include <fstream>
#include "../parser/node.h"

void obfuscateNames(Node* program, std::string symbolFname);
void checkUnObfuscated(Node* node);
