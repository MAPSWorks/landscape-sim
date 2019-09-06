#include "i_camera.h"

namespace scene {
ICamera::ICamera(t::Vec3 position) : world_position_(position) {
}

void ICamera::SetWorldPosition(t::Vec3 position) {
    world_position_ = position;
}

const t::Vec3& ICamera::GetWorldPosition() const {
    return world_position_;
}
}; // scene