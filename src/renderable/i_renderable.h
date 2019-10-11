#pragma once
#include <base/types.h>
#include <renderer/vlk/command_buffer.h>
#include <renderer/vlk/uniform_buffer.h>
#include <renderer/frame_manager.h>

// Renderables base class
namespace renderable {
class IRenderable {
public:
    IRenderable();
    // Deleting a derived class renderable using a pointer to a base class
    // that has a non-virtual destructor results in undefined behavior
    virtual ~IRenderable() = default;
    // This function is called after descriptor pool is created.
    // So we can add and update descriptor sts.
    virtual void InitDescriptorSets() = 0;
    // Write command to given command buffer that is already in recording state
    // frame_id - id of a current frame-in-flight
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameManager::FrameId frame_id) const = 0;
    // frame_id - id of a current frame-in-flight
    virtual void UpdateUniformBuffer(renderer::FrameManager::FrameId frame_id) const = 0;
};
}; // renderable