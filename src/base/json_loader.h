#pragma once
#include <string>
#include <external/json/json.hpp>

// Parses a given json file and lets to retrieve an elements by type
// Uses nlohmann json c++ library internally for parsing
namespace base {
class JSONLoader {
public:
    using JsonType = nlohmann::json;
    JSONLoader(const std::string& file_name);
    // Data in structure is acessed by at() and get() methods
    // Example: at("field1").at("sub_field1").get<std::string>();
    const JsonType Get() const;
private:
    JsonType Init(const std::string& file_name) const;
    // Handle to parsed data structure
    JsonType parsed_data_;
};
};