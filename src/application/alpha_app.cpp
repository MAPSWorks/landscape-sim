#include "alpha_app.h"
#include <base/log.h>

// TODO: Global data, probably should be in json 
const t::Size window_size(800, 600);
const renderer::vlk::Settings renderer_settings{ "Alpha app", 1, "W2GPU", 1 };
const scene::Settings scene_settings{ };
// Objects and positions and camera position are all initial and can change
// while scene is running
const scene::ObjectDescription triangle_description{ //object::Type::kTriangle, t::Vec3(0.0),
                                                      object::Type::kTriangleVB, t::Vec3(0.0),
                                                    };
const std::vector<scene::ObjectDescription> scene_objects{ triangle_description };
const scene::Description scene_description{ t::Vec3(0.0) , scene_objects };

namespace application {
AlphaApp::AlphaApp() : platform::IApplication(window_size),
    renderer_(renderer_settings, GetWindowHandle()),
    scene_manager_(scene_settings, scene_description, renderer_) {
    base::Log::Info("Alpha app initialized");
}

AlphaApp::~AlphaApp() {
    base::Log::Info("Alpha app shutting down...");
}

void AlphaApp::Update() {
}

void AlphaApp::RenderFrame() {
    scene_manager_.RenderFrame();
}

void AlphaApp::Resize(const t::Size& win_size) {
    renderer_.Resize();
}

// Right after exiting main loop and before exiting app
void AlphaApp::OnExit() const {
    renderer_.WaitForGPUIdle();
}
}; // application