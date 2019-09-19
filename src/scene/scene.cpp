#include "scene.h"
#include <base/log.h>
#include "scene_parser.h"

namespace scene {
Scene::Scene(const std::string& file_name) : 
    contents_(GenerateContents(file_name)) {
    base::Log::Info("Scene: scene initialized");
}

const Scene::Contents& Scene::GetContents() const {
    return contents_;
}

Scene::Contents Scene::GenerateContents(const std::string& file_name) const{
    SceneParser scene_parser(file_name);
    Contents contents;
    contents.camera = scene_parser.GetCamera();
    contents.renderables = scene_parser.GetRenderables();
    return contents;
}
}; // scene