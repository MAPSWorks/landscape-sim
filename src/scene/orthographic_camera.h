#pragma once
#include <base/types.h>
#include "i_camera.h" 

// Orthographic cameras, where the viewing volume is a rectangular box.
namespace scene {
class OrthographicCamera : public ICamera {
public:
    // To initialize camera
    struct Parameters {
        // Actual position of camera in world space
        t::Vec3 world_translation;
        t::Real32 znear;
        t::Real32 zfar;
        // Magnification of camera
        t::Real32 xmag;
        t::Real32 ymag;
    };
    OrthographicCamera(Parameters params);
    void Update() override;
private:
    // Camera matrix parameters
    t::Real32 z_near_;
    t::Real32 z_far_;
    t::Real32 x_mag_;
    t::Real32 y_mag_;
};
}; // scene