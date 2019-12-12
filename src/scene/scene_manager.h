#pragma once
#include <string_view>
#include <renderer/renderer.h>
#include <platform/input.h>
#include "scene.h"
#include "view.h"

// Top-level renderable of the engine - brings otherwise unrelated parts together
// to initialize, update and render the scene
namespace scene {
class SceneManager {
public:
    SceneManager(renderer::Renderer &renderer, std::string_view scene_file_name);
    // input - current state of user input (keyboard, mouse)
    void Update(const platform::Input& input) const;
    void RenderFrame() const;
    void MouseMove(const platform::Input& input) const;
private:
    // Renderer will not be changed in runtime therefore grab the reference
    renderer::Renderer& renderer_;
    // Per-view resources
    View view_;
    // Scene to be managed
    const Scene scene_;
};
}; // scene