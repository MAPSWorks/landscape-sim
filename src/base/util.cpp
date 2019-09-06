#include "util.h"

namespace base {
std::string StripIlligallChars(const std::string& text) {
    std::string stripped_string = text;
    const std::string illigal_chars = "\\/:?\"<>| ";
    for (auto it = stripped_string.begin(); it < stripped_string.end(); ++it) {
        bool found = (illigal_chars.find(*it) != std::string::npos);
        if (found) {
            *it = '_';
        }
    }
    return stripped_string;
}
}; // base