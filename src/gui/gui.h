#pragma once
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <imgui/imgui.h>
#include "imgui_impl_vulkan.h"

// Imgui library implementation wrapper.
// Class does NOT implement RAII, gui is initialized in steps.
// First initialize platform, then renderer.
namespace gui {
class GUI {
public:
    GUI(bool enabled);
    // Initialize GLFW platform
    void InitPlatform(GLFWwindow* window, bool install_callbacks) const;
    // Init renderer binding
    // renderer_info - list of objects needed to initialize renderer
    // render_pass - a render pass to render a gui
    void InitRenderer(ImGui_ImplVulkan_InitInfo& init_info, VkRenderPass render_pass) const;
private:
    const bool enabled_;
};

}; // gui