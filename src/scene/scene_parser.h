#pragma once
#include <string>
#include <memory>
#include <base/json_loader.h>
#include "i_camera.h"

// Parses scene json file and return scene contents
namespace scene {
class SceneParser {
public:
    SceneParser(const std::string& file_name);
    // Return pointer to camera object
    std::unique_ptr<ICamera> GetCamera() const;
private:
    // Parses given file and stores parsed data
    base::JSONLoader scene_loader_;
};
};