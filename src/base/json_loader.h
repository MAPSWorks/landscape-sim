#pragma once
#include <string>
#include <external/json/json.hpp>

// Parses a given json file and lets to retrieve an elements by type
// Uses nlohmann json c++ library internally for parsing
namespace base {
class JSONLoader {
public:
    JSONLoader(const std::string& file_name);
    // Data in structure is acessed by at() and get() methods
    // Example: at("field1").at("sub_field1").get<std::string>();
    const nlohmann::json Get() const;
private:
    nlohmann::json Init(const std::string& file_name) const;
    // Handle to parsed data structure
    nlohmann::json parsed_data_;
};
};