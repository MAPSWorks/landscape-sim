#pragma once
#include <string>
#include <memory>
#include "i_camera.h"

// Scene content storage and manipulation
namespace scene {
class Scene {
public:
    // POD structure of a contents that describe single scene
    struct Contents {
        std::unique_ptr<ICamera> camera;
    };
    Scene(const std::string& file_name);
    const Contents& GetContents() const;
    // Generate scene objects and structure from given parameters, return the contents
    Contents GenerateContents(const std::string& file_name) const;
private:
    // Contents potentially change
    Contents contents_;
};
}; // scene