#pragma once
#include <base/types.h>
#include <platform/input.h>

// Interface for camera objects
namespace scene {
class ICamera {
public:
    ICamera(t::Vec3 position);
    // Common public methods
    void SetWorldPosition(t::Vec3 position);
    const t::Vec3& GetWorldPosition() const;
    // Update state of a camera
    virtual void Update(const platform::Input& input) = 0;
    virtual t::Mat4 GetViewMatrix() const = 0;
protected:
    // Private variables common for all camera types
    t::Vec3 world_position_;
};
}; // scene