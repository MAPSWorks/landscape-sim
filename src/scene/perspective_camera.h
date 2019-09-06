#pragma once
#include <base/types.h>
#include "i_camera.h" 

// Perspective camera, where the viewing volume is a truncated pyramid
namespace scene {
class PerspectiveCamera : public ICamera {
public:
    // To initialize camera
    struct Parameters {
        // Actual position of camera in world space
        t::Vec3 world_translation;
        t::Real32 znear;
        t::Real32 zfar;
        // Vertical field of view
        t::Real32 yfov;
    };
    PerspectiveCamera(Parameters params);
    void Update() override;
private:
    /*
    // If some of these are shared acroos various camera types, bring them to base class 
    // Vector of view direction
    t::Vec3 front_;
    // Vector upside up
    t::Vec3 up_;
    // Vector to the right of eyes
    t::Vec3 right_;
    */
    // Camera matrix parameters
    t::Real32 z_near_;
    t::Real32 z_far_;
    // Vertical field of view, for perspective type only
    t::Real32 y_field_of_view_;
};
}; // scene