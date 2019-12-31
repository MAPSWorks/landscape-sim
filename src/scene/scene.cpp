#include "scene.h"
#include <base/log.h>
#include "scene_parser.h"

namespace scene {
Scene::Scene(std::string_view file_name, renderer::Renderer& renderer, const View& view) :
    contents_(GenerateContents(file_name, renderer, view)) {
    base::Log::Info("Scene: scene initialized");
}

Scene::Contents& Scene::GetContents() {
    return contents_;
}

Scene::Contents Scene::GenerateContents(std::string_view file_name, renderer::Renderer& renderer, const View& view) const{
    SceneParser scene_parser(file_name);
    Contents contents;
    contents.camera = scene_parser.GetCamera();
    contents.renderables = scene_parser.GetRenderables(renderer, view);
    return contents;
}
}; // scene