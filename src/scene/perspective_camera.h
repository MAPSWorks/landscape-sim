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
        t::F32 znear;
        t::F32 zfar;
        // Vertical field of view (radians)
        t::F32 yfov;
    };
    PerspectiveCamera(const Parameters& params);
    void Move(const platform::Input& input, t::F32 dt) override;
    // Camera free-look
    // constrain_pitch - whether free look is constrained
    void Rotate(const platform::Input& input, bool constrain_pitch = true) override;
    virtual t::Mat4 GetViewMatrix() const override;
    virtual t::Mat4 GetProjectionMatrix(t::F32 aspect_ratio) const override;
private:
    // Possible movement directions
    enum class Direction {
        kForward,
        kBackward,
        kLeft,
        kRight,
        kUp,
        kDown
    };
    // Update camera vector based on Euler angles
    void UpdateVectors(t::F32 yaw, t::F32 pitch);
    // Move in given direction.
    // Mediated by delta time.
    void UpdatePosition(Direction direction, t::F32 dt);

    // Vector of view direction
    t::Vec3 front_;
    // Vector upside up
    t::Vec3 up_;
    // Vector to the right of eyes
    t::Vec3 right_;
    // Camera matrix parameters
    t::F32 z_near_;
    t::F32 z_far_;
    // Vertical field of view (radians), for perspective type only
    t::F32 y_field_of_view_;
    // Eular Angles of camera orientation
    t::F32 yaw_;
    t::F32 pitch_;
    // Camera options
    t::F32 movement_speed_;
    t::F32 mouse_sensitivity_;
};
}; // scene