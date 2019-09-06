#pragma once
#include <vector>
#include <renderer/renderer.h>
#include "scene.h"
#include "types.h"

// Top-level renderable of the engine - brings otherwise unrelated parts together
// to initialize, update and render the scene
namespace scene {
class SceneManager {
public:
    SceneManager(renderer::Renderer &renderer, const Scene& scene);
    ~SceneManager();
    void Update() const;
    void RenderFrame() const;
private:
    // Initialize objects given at scene creation time
    UniqueObjectVector InitObjects(const std::vector<ObjectDescription>& object_descriptions);
    // Renderer will not be changed in runtime therefore grab the reference
    renderer::Renderer& renderer_;
    // Reference to scene manager will manage
    const Scene& scene_;
};
}; // scene