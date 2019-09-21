#pragma once
#include <string>
#include <memory>
#include <vector>
#include <renderer/renderer.h>
#include "i_camera.h"
#include <renderable/i_renderable.h>

// Scene content storage and manipulation
namespace scene {
class Scene {
public:
    using RenderableVector = std::vector<std::unique_ptr<renderable::IRenderable>>;
    // POD structure of a contents that describe single scene
    struct Contents {
        // Scene vantage pont
        std::unique_ptr<ICamera> camera;
        // Visible scene objects
        RenderableVector renderables;
    };
    Scene(const std::string& file_name, renderer::Renderer& renderer);
    const Contents& GetContents() const;
    // Generate scene objects and structure from given scene file, return the contents
    Contents GenerateContents(const std::string& file_name, renderer::Renderer& renderer) const;
private:
    // Contents potentially change
    Contents contents_;
};
}; // scene