#include "gui.h"
#include <base/log.h>
#include "imgui_impl_glfw.h"

namespace gui {
GUI::GUI(bool enabled) : 
    enabled_(enabled) {
    if (enabled_) {
        base::Log::Info("GUI: on");
    }
    else {
        base::Log::Info("GUI: off");
    }
}

void GUI::InitPlatform(GLFWwindow* window, bool install_callbacks) const {
    if (enabled_) {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        // Supress warning in case io is not used, if used REMOVE this line
        (void)io;
        //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
        // Setup Dear ImGui style
        ImGui::StyleColorsDark();
        //ImGui::StyleColorsClassic();
        // Setup Platform bindings
        ImGui_ImplGlfw_InitForVulkan(window, true);
    }
}

void GUI::InitRenderer(ImGui_ImplVulkan_InitInfo& init_info, VkRenderPass render_pass) const {
    if (enabled_) {
        ImGui_ImplVulkan_Init(&init_info, render_pass);
    }
}

}; // gui