#pragma once
#include <base/types.h>
#include <renderer/vlk/command_buffer.h>

// Renderables base class
namespace renderable {
class IRenderable {
public:
    IRenderable();
    // Deleting a derived class renderable using a pointer to a base class
    // that has a non-virtual destructor results in undefined behavior
    virtual ~IRenderable() {};
    // Write command to given command buffer that is already in recording state
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const = 0;
private: // TODO: probably should be protected
    t::Vec3 world_position_;
};
}; // renderable