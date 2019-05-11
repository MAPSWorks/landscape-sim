#pragma once
#include <fstream>
#include <vector>
#include <exception>

// Various helper fimctions without home yet
namespace util {
// Read binary file (currently to load SPIR-V shaders)
std::vector<char> ReadBinaryFile(const std::string& filename) {
    std::ifstream file(filename, std::ios::ate | std::ios::binary);
    if (!file.is_open()) {
        throw std::runtime_error("Util: failed to open file " + filename);
    }
    // We are at the back so we can tell size of a file and allocate buffer
    size_t file_size = (size_t)file.tellg();
    std::vector<char> buffer(file_size);
    // Go to beginning and read all at once
    file.seekg(0);
    file.read(buffer.data(), file_size);
    file.close();
    return buffer;
}
};