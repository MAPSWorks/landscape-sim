#pragma once
#include <base/types.h>
#include <renderer/vlk/command_buffer.h>
#include <renderer/vlk/uniform_buffer.h>

// Renderables base class
namespace renderable {
class IRenderable {
public:
    IRenderable();
    // Deleting a derived class renderable using a pointer to a base class
    // that has a non-virtual destructor results in undefined behavior
    virtual ~IRenderable() = default;
    // Write command to given command buffer that is already in recording state
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const = 0;
    virtual void UpdateUniformBuffer(const renderer::vlk::UniformBuffer& cuniform_buffer) const = 0;
};
}; // renderable