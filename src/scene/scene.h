#pragma once
#include <string_view>
#include <memory>
#include <vector>
#include <renderer/renderer.h>
#include <renderable/i_renderable.h>
#include "i_camera.h"
#include "view.h"

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
    Scene(std::string_view file_name, renderer::Renderer& renderer, const View& view);
    const Contents& GetContents() const;
    // Generate scene objects and structure from given scene file, return the contents
    Contents GenerateContents(std::string_view file_name, renderer::Renderer& renderer, const View& view) const;
private:
    // Contents potentially change
    Contents contents_;
};
}; // scene