#pragma once
#include <string>
#include "types.h"

// Various utility functions
namespace base {
// Strip given string from illigal characters.
// Meant for file names sfety.
std::string StripIlligallChars(const std::string& text);
}; // base