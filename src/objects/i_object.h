#pragma once
#include <base/types.h>
#include <renderer/vlk/command_buffer.h>

namespace object {
// Renderable object base class
class IObject {
public:
    IObject();
    // Deleting a derived class object using a pointer to a base class
    // that has a non-virtual destructor results in undefined behavior
    virtual ~IObject() {};
    // Write command to given command buffer that is already in recording state
    virtual void AppendCommandBuffer(const renderer::vlk::CommandBuffer& command_buffer) const = 0;
private:
    t::Vec3 world_position_;
};
}; // object