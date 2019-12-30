#pragma once
#include <base/types.h>

// Represents environment of the scene - sunlight direction, wind etc.
namespace scene {
class Environment {
public:
    Environment();
    const t::Vec3& GetSunlightDirection() const;
    // Prepare user interface to modify environment values.
    void UpdateGUI() const;
private:
    // Direction vector from sunlight to origin
    t::Vec3 sunlight_direction_;
};
}; //  scene