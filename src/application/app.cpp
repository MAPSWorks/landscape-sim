#include "app.h"
#include <vector>
#include <base/log.h>
#include <base/types.h>
#include <renderer/vlk/types.h>
#include <objects/types.h>
#include <scene/types.h>

// TODO: Global data, probably should be in json 
const t::Size window_size(800, 600);
const renderer::vlk::Settings renderer_settings{ "Test app", 1, "W2GPU", 1 };
const scene::Settings scene_settings{ };
// Objects and positions and camera position are all initial and can change
// while scene is running
const scene::ObjectDescription triangle_description{ object::Type::kTriangle, t::Vec3(0.0) };
const std::vector<scene::ObjectDescription> scene_objects{ triangle_description };
const scene::Description scene_description{ t::Vec3(0.0) , scene_objects };

namespace application {
App::App() :
    platform_(window_size),
    renderer_(renderer_settings, platform_.GetWindowHandle()),
    scene_manager_(scene_settings, scene_description, renderer_) {
    base::Log::Info("App initialized");
}

App::~App() {
    base::Log::Info("App shutting down...");
}

void App::Run() {
    while (platform_.IsRunning()) {
        platform_.PollEvents();
        scene_manager_.RenderFrame();
    }
    renderer_.WaitForGPUIdle();
}
}; // application
