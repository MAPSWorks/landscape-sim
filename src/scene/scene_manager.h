#pragma once
#include <renderer/renderer.h>
#include "scene.h"

#include <renderer/vlk/descriptor_set_layout.h>
#include <renderer/vlk/pipeline_layout.h>

// Top-level renderable of the engine - brings otherwise unrelated parts together
// to initialize, update and render the scene
namespace scene {
struct UniformBufferView {
    t::Mat4 view_from_world;
    t::Mat4 projection_from_view;
};
class SceneManager {
public:
    SceneManager(renderer::Renderer &renderer, const Scene& scene);
    void Update() const;
    void RenderFrame() const;
private:
    const renderer::vlk::DescriptorSetLayout& InitDescriptorSetLayout() const;
    void UpdateUniformBuffer(t::U32 frame_id) const;

    // Renderer will not be changed in runtime therefore grab the reference
    renderer::Renderer& renderer_;


    const renderer::vlk::DescriptorSetLayout& descriptor_set_layout_view;
    const t::U64 uniform_buffer_id;
    t::U64 descriptor_set_view_id;
    const renderer::vlk::PipelineLayout dummy_view_pipeline_layout;


    // Reference to scene manager will manage
    const Scene& scene_;

};
}; // scene