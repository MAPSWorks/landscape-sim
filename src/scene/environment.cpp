#include "environment.h"
#include <imgui/imgui.h>

namespace scene {
Environment::Environment() :
    sunlight_direction_(0.0, 0.0, 1.0) {
}

const t::Vec3& Environment::GetSunlightDirection() const {
    return sunlight_direction_;
}

void Environment::UpdateGUI() const {
    ImGui::Begin("Environment");

    ImGui::End();
}
}; //  scene