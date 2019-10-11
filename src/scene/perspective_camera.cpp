#include "perspective_camera.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <base/log.h>

namespace scene {
PerspectiveCamera::PerspectiveCamera(const PerspectiveCamera::Parameters& params) :
    ICamera((params.world_translation)),
    z_near_(params.znear),
    z_far_(params.zfar),
    y_field_of_view_(params.yfov),
    // Points at negative Z axis by default
    yaw_(0),
    pitch_(0),
    // 1.4 m/s human walking speed
    movement_speed_(1.4f) {
    UpdateVectors(yaw_, pitch_);
    base::Log::Info("Scene: perspective camera initialized : position - " , glm::to_string(world_position_), ", znear - ",
        z_near_, ", zfar - ", z_far_, ", yfov - ", y_field_of_view_);
}

void PerspectiveCamera::Update(const platform::Input& input) {
    t::F32  dt = 0.01f;
    // Camera controls using active keys (keys neing currently held)
    if (input.GetKeyData().active.count(platform::Input::Key::kW))
        Move(Direction::kForward, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kS))
        Move(Direction::kBackward, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kA))
        Move(Direction::kLeft, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kD))
        Move(Direction::kRight, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kR))
        Move(Direction::kUp, dt);
    if (input.GetKeyData().active.count(platform::Input::Key::kF))
        Move(Direction::kDown, dt);
}

t::Mat4 PerspectiveCamera::GetViewMatrix() const {
    return glm::lookAt(world_position_, world_position_ + front_, up_);
}

// Calculate front, right and up vectors from current Euler angles
void PerspectiveCamera::UpdateVectors(t::F32 yaw, t::F32 pitch) {
    // Y-axis is up
    t::Vec3 world_up(0.0, 1.0, 0.0);
    // Calculate the new Front vector
    t::Vec3 front;
    front.x = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    front.y = sin(glm::radians(pitch));
    front.z = -cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    front_ = glm::normalize(front);
    // Also re-calculate the Right and Up vector
    right_ = glm::normalize(glm::cross(front_, world_up));
    up_ = glm::normalize(glm::cross(right_, front_));
}

void PerspectiveCamera::Move(Direction direction, t::F32 dt) {
    // Since this is animation, mediate it by elapsed time
    t::F32 velocity = movement_speed_ * dt;
    switch (direction) {
    case Direction::kForward:
        world_position_ += front_ * velocity;
        break;
    case Direction::kBackward:
        world_position_ -= front_ * velocity;
        break;
    case Direction::kLeft:
        world_position_ -= right_ * velocity;
        break;
    case Direction::kRight:
        world_position_ += right_ * velocity;
        break;
    case Direction::kUp:
        world_position_ += up_ * velocity;
        break;
    case Direction::kDown:
        world_position_ -= up_ * velocity;
        break;
    }
}
}; // scene