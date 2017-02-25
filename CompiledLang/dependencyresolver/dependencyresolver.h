#pragma once

#include <unordered_map>
#include "../parser/node.h"

std::unordered_multimap<std::string, FunctionPointer> resolveDependencies();
