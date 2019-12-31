#include "environment.h"
#include <glm/gtx/rotate_vector.hpp>
#include <imgui/imgui.h>

namespace scene {
Environment::Environment() :
    sunlight_direction_(0.0, 0.0, 1.0) {
}

const t::Vec3& Environment::GetSunlightDirection() const {
    return sunlight_direction_;
}

void Environment::UpdateGUI() {
    ImGui::Begin("Environment");
    ImGui::Text("Sunlight direction: (%0.2f, %0.2f, %0.2f)", sunlight_direction_.x, sunlight_direction_.y, sunlight_direction_.z);
    static t::F32 vertical_angle = 0;
    ImGui::SliderFloat("Vertical angle ", &vertical_angle, -0.5f, (t::F32)t::kPi2);
    static t::F32 horizontal_angle = 0;
    ImGui::SliderFloat("Horizontal angle ", &horizontal_angle, 0.0f, (t::F32)t::k2Pi);
    // anges 0 and 0 give light direction of (0,0,1)
    sunlight_direction_ = glm::rotateX(t::Vec3(0.0, 0.0, 1.0), vertical_angle);
    sunlight_direction_ = glm::rotateY(sunlight_direction_, horizontal_angle);
    ImGui::End();
}
}; //  scene