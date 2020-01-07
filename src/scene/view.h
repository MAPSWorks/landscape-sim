#pragma once
#include <base/types.h>
#include <renderer/vlk/descriptor_set_layout.h>
#include <renderer/vlk/pipeline_layout.h>
#include <renderer/renderer.h>
#include <renderer/shader_resources.h>
#include "environment.h"
#include "i_camera.h"

// Represents/abstract a view of the scene.
// View is concept at a highest level of a rendering hot loop.
// A frame rendering happens per-view.
namespace scene {
class View {
public:
    struct UniformData {
        t::Mat4 view_from_world;
        t::Mat4 projection_from_view;
        t::Vec3 sunlight_direction;
        // allignas -  variable is aligned on 16 byte boundaries
        alignas(16) t::Vec3 sunlight_color;
    };
    View(renderer::Renderer& renderer);
    // After descriptor pool has been created, we can allocate descriptor sets
    void InitDescriptorSet();
    // Update uniform buffer for given frame-in-flight.
    // Camera represents current view.
    void UpdateUniformBuffer(renderer::FrameManager::FrameId frame_id, const ICamera& camera, const Environment& environmen) const;
    // Bind per-view descritor set to command buffer with dummy layout
    void BindDescriptorSet(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameManager::FrameId frame_id) const;
    const renderer::vlk::DescriptorSetLayout& GetDescriptorSetLayout() const;
private:
    const renderer::vlk::DescriptorSetLayout& AddDescrSetLayout() const;
    // Add buffer to shader resources and return it id
    t::U64 AddUniformBufferId() const;
    // Reference to renderer object
    renderer::Renderer& renderer_;
    // Per-view descriptor set layout
    const renderer::vlk::DescriptorSetLayout& descr_set_layout_;
    // Uniform buffer data are stored in a buffer pointed to by this id
    const renderer::ShaderResources::UniformBufferId uniform_buffer_id_;
    // Descriptor set should be bound together with pipeline layout, therefore
    // if we want to bind per-view shader resource before-hand, we need a dummy pipeline layout.
    // Per-view shader data in actual objects will be exactly like in this dummy layout plus their own.
    const renderer::vlk::PipelineLayout pipeline_layout_dummy_;
    // The per-view descriptor set is pointed to by this id
    renderer::ShaderResources::DescrSetId descr_set_id_;
};
}; // scene