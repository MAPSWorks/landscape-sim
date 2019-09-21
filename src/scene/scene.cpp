#include "scene.h"
#include <base/log.h>
#include "scene_parser.h"

namespace scene {
Scene::Scene(const std::string& file_name, renderer::Renderer& renderer) :
    contents_(GenerateContents(file_name, renderer)) {
    base::Log::Info("Scene: scene initialized");
}

const Scene::Contents& Scene::GetContents() const {
    return contents_;
}

Scene::Contents Scene::GenerateContents(const std::string& file_name, renderer::Renderer& renderer) const{
    SceneParser scene_parser(file_name);
    Contents contents;
    contents.camera = scene_parser.GetCamera();
    contents.renderables = scene_parser.GetRenderables(renderer);
    return contents;
}
}; // scene