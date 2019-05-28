#pragma once
#include <base/types.h>
#include <renderer/vlk/command_buffers.h>

namespace object {
// Renderable object base class
class IObject {
public:
    IObject();
    // Deleting a derived class object using a pointer to a base class
    // that has a non-virtual destructor results in undefined behavior
    virtual ~IObject() {};
    // Add command to  command buffer with given index, buffer is already started
    virtual void RecordToCommandBuffer(uint32_t buffer_index) const = 0;
private:
    t::Vec3 world_position_;
};
}; // object