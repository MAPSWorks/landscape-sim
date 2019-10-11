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
        t::F32 znear;
        t::F32 zfar;
        // Magnification of camera
        t::F32 xmag;
        t::F32 ymag;
    };
    OrthographicCamera(const Parameters& params);
    void Update(const platform::Input& input) override;
private:
    // Camera matrix parameters
    t::F32 z_near_;
    t::F32 z_far_;
    t::F32 x_mag_;
    t::F32 y_mag_;
};
}; // scene