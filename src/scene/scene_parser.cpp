#include "scene_parser.h"
#include <stdexcept>
#include <base/log.h>
#include "perspective_camera.h"
#include "orthographic_camera.h"
#include <renderable/terrain.h>

namespace scene {
SceneParser::SceneParser(const std::string& file_name) : 
    loader_(file_name),
    data_catche_(loader_.Get()) {
    base::Log::Info("Scene: scene parser initialized from file '",file_name,"'");
}

// Camera can be of different type
std::unique_ptr<ICamera> SceneParser::GetCamera() const {
    std::unique_ptr<ICamera> camera;
    const base::JSONLoader::JsonType& scene_data_camera = data_catche_.at("camera");
    const std::string camera_type = scene_data_camera.at("type").get<std::string>();
    const std::vector<t::F32> camera_translation = scene_data_camera.at("translation");
    // Perspective camera
    if (camera_type == "perspective") {
        PerspectiveCamera::Parameters parameters;
        parameters.world_translation = t::Vec3(camera_translation.at(0), camera_translation.at(1),
            camera_translation.at(2));
        parameters.znear = scene_data_camera.at("perspective").at("znear").get<t::F32>();
        parameters.zfar = scene_data_camera.at("perspective").at("zfar").get<t::F32>();
        parameters.yfov = scene_data_camera.at("perspective").at("yfov").get<t::F32>();
        camera = std::make_unique<PerspectiveCamera>(parameters);
    }
    // Orthographic camera
    else if (camera_type == "orthographic") {
        OrthographicCamera::Parameters parameters;
        parameters.world_translation = t::Vec3(camera_translation.at(0), camera_translation.at(1),
            camera_translation.at(2));
        parameters.znear = scene_data_camera.at("orthographic").at("znear").get<t::F32>();
        parameters.zfar = scene_data_camera.at("orthographic").at("zfar").get<t::F32>();
        parameters.xmag = scene_data_camera.at("orthographic").at("xmag").get<t::F32>();
        parameters.ymag = scene_data_camera.at("orthographic").at("ymag").get<t::F32>();
        camera = std::make_unique<OrthographicCamera>(parameters);
    }
    else {
        throw std::runtime_error("Scene: unrecognized camera type '" + camera_type + "'");
    }
    return camera;
}

// Add renderable objects
Scene::RenderableVector SceneParser::GetRenderables(renderer::Renderer& renderer) const {
    Scene::RenderableVector renderables;
    // Terrain, if is defined
    if (data_catche_.find("terrain") != data_catche_.end()) {
        const auto& scene_data_terrain = data_catche_.at("terrain");
        renderables.push_back(std::make_unique<renderable::Terrain>(renderer));
    }
    return renderables;
}
};