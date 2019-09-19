#pragma once
#include <string>
#include <memory>
#include <base/json_loader.h>
#include "scene.h"
#include "i_camera.h"

// Parses scene json file and return scene contents
// Upon initialization scene data is catched and each individual getter gets
// requested data as needed.
namespace scene {
class SceneParser {
public:
    // Loads scene data from file into catche
    // file_name - File name of the scene
    SceneParser(const std::string& file_name);
    // Return pointer to derived camera object
    std::unique_ptr<ICamera> GetCamera() const;
    // Returns list of renderable scene objects
    Scene::RenderableVector GetRenderables() const;
private:
    // Parses given file and stores parsed data
    const base::JSONLoader loader_;
    // Reference to data loaded from loader
    const base::JSONLoader::JsonType& data_catche_;
};
};