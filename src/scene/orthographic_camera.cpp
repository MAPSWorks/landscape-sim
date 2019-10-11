#include "orthographic_camera.h"
#include <glm/gtx/string_cast.hpp>
#include <base/log.h>

namespace scene {
OrthographicCamera::OrthographicCamera(const OrthographicCamera::Parameters& params) :
    ICamera((params.world_translation)),
    z_near_(params.znear),
    z_far_(params.zfar),
    x_mag_(params.xmag),
    y_mag_(params.ymag) {
    base::Log::Info("Scene: perspective camera initialized : position - ", glm::to_string(world_position_), ", znear - ",
        z_near_, ", zfar - ", z_far_, ", xmag - ", x_mag_, ", ymag - ", y_mag_);
}

void OrthographicCamera::Update(const platform::Input& input) {

}

t::Mat4 OrthographicCamera::GetViewMatrix() const
{
    return t::Mat4();
}

};