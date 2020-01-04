#include "environment.h"
#include <imgui/imgui.h>

namespace scene {
Environment::Environment()  {
}

const t::Vec3& Environment::GetSunlightDirection() const {
    return sun_.direction;
}

void Environment::Update(t::F32 dt) {
    sun_.RotateDirection();
}

void Environment::UpdateGUI() {
    ImGui::Begin("Environment");
    ImGui::Text("Sunlight direction: (%0.2f, %0.2f, %0.2f)", sun_.direction.x, sun_.direction.y, sun_.direction.z);
    ImGui::SliderFloat("Pitch angle ", &sun_.pitch_angle, 0.5f, (t::F32)-t::kPi2);
    ImGui::SliderFloat("Head angle ", &sun_.head_angle, 0.0f, (t::F32)t::k2Pi);
    ImGui::End();
}
}; //  scene