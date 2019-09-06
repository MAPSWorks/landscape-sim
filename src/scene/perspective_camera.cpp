#include "perspective_camera.h"
#include <glm/gtx/string_cast.hpp>
#include <base/log.h>

namespace scene {
PerspectiveCamera::PerspectiveCamera(PerspectiveCamera::Parameters params) :
    ICamera((params.world_translation)),
    z_near_(params.znear),
    z_far_(params.zfar),
    y_field_of_view_(params.yfov) {
    base::Log::Info("Scene: perspective camera initialized : position - " , glm::to_string(world_position_), ", znear - ",
        z_near_, ", zfar - ", z_far_, ", yfov - ", y_field_of_view_);
}

void PerspectiveCamera::Update()
{
    
}
}; // scene