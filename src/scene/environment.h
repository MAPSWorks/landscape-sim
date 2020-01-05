#pragma once
#include <glm/gtx/rotate_vector.hpp>
#include <base/types.h>

// Represents environment of the scene - sunlight direction, wind etc.
// TODO: environemnt is a candidate for a separate namespace
namespace scene {
// The sun is represented as a directional light
struct Sun {
    // Position sunlight accordint to Euler direction angles
    void RotateDirection() {
        direction = glm::rotateX(kDefaultDirection, pitch_angle);
        direction = glm::rotateY(direction, head_angle);
    }
    // By default oriented along -Z axis
    const t::Vec3 kDefaultDirection = t::Vec3(0.0, 0.0, -1.0);
    // Sunlight color
    t::Vec3 color = t::Vec3(1.0, 1.0, 1.0);
    // Direction vector from sunlight to origin
    t::Vec3 direction = kDefaultDirection;
    // Euler rotation angles of a direction
    // About Y axis
    t::F32 head_angle = 0;
    // About X axis
    t::F32 pitch_angle = 0;
};

class Environment {
public:
    Environment();
    const Sun& GetSun() const;
    // Update environemnt state
    void Update(t::F32 dt);
    // Prepare user interface to modify environment values.
    void UpdateGUI();
private:
    Sun sun_;
};
}; //  scene