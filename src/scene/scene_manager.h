#pragma once
#include <vector>
#include <base/types.h>
#include <renderer/renderer.h>
#include "types.h"

namespace scene {
// Top-level object of the engine - brings otherwise unrelated objets together
// to initialize, update and render the scene
class SceneManager {
public:
    SceneManager(const Settings& settings,  const Description &scene_description, 
        const renderer::Renderer &renderer);
    ~SceneManager();
    void RenderFrame() const;
private:
    // Initialize objects given at scene creation time
    UniqueObjectVector InitObjects(const std::vector<ObjectDescription>& object_descriptions);
    // Renderer will not be changed in runtime therefore grab the reference
    const renderer::Renderer& renderer_;
    t::Vec3 camera_world_position_;
    // Stores all renderable objects in the scene, is not constant
    // because can potentially change during rendering (add new objects or remove)
    // Therefor objects that are passed through scene constructur are only initial
    UniqueObjectVector objects_;
};
}; // scene