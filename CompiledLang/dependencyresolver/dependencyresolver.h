#pragma once

#include <unordered_map>
#include "../parser/node.h"

std::unordered_map<std::string, FunctionPointer> resolveDependencies();
