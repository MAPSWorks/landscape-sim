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
    virtual void Move(const platform::Input& input, t::F32 dt) = 0;
    virtual void Rotate(const platform::Input& input, bool constrain_pitch = true) = 0;
    virtual t::Mat4 GetViewMatrix() const = 0;
    // aspect_ratio - aspect ratio of current viewinf screen
    virtual t::Mat4 GetProjectionMatrix(t::F32 aspect_ratio) const = 0;
protected:
    // Private variables common for all camera types
    t::Vec3 world_position_;
};
}; // scene