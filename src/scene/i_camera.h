#pragma once
#include <base/types.h>

// Interface for camera objects
namespace scene {
class ICamera {
public:
    ICamera(t::Vec3 position);
    // Common public methods
    void SetWorldPosition(t::Vec3 position);
    const t::Vec3& GetWorldPosition() const;
    // TODO: change this update function to something usefull!
    virtual void Update() = 0;
protected:
    // Private variables common for all camera types
    t::Vec3 world_position_;
};
}; // scene