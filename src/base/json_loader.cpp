#include "json_loader.h"
#include <fstream>
#include <stdexcept>
#include <base/log.h>

namespace base {
JSONLoader::JSONLoader(std::string_view file_name) : 
    parsed_data_(Init(file_name.data())) {
    base::Log::Info("Base: parsing of '", file_name, "' complete");
}

const JSONLoader::JsonType& JSONLoader::Get() const {
    return parsed_data_;
}

// NOTE: string_view could have been passed if: ifstream accepted it (this can be solved through data() f-tion)
//       but the main problem is lack of + operator for string view to use it in exception description, 
//       maybe this will change in the future.
JSONLoader::JsonType JSONLoader::Init(const std::string& file_name) const {
    base::Log::Info("Base: parsing json file '", file_name, "' ...");
    std::ifstream file_stream(file_name, std::ifstream::in);
    if (file_stream.fail()) {
        throw std::runtime_error("Base: error opening file '" + file_name + "'");
    }
    // Throws if error while parsing
    return nlohmann::json::parse(file_stream);;
}
};