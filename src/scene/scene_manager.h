#pragma once
#include <renderer/renderer.h>
#include "scene.h"
#include "view.h"

// Top-level renderable of the engine - brings otherwise unrelated parts together
// to initialize, update and render the scene
namespace scene {
class SceneManager {
public:
    SceneManager(renderer::Renderer &renderer, const Scene& scene);
    void Update() const;
    void RenderFrame() const;
private:
    // Renderer will not be changed in runtime therefore grab the reference
    renderer::Renderer& renderer_;
    // Reference to scene manager will manage
    const Scene& scene_;
    // Per-view resources
    View view_;
};
}; // scene