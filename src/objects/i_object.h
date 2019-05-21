#pragma once
#include <base/types.h>

namespace object {
// Renderable object base class
class IObject {
public:
    IObject();
    // Deleting a derived class object using a pointer to a base class
    // that has a non-virtual destructor results in undefined behavior
    virtual ~IObject() {};
    virtual void RenderFrame() const = 0;
private:
    t::Vec3 world_position_;
};
}; // object