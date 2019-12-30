#pragma once
#include <memory>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <renderer/renderer.h>
#include <imgui/imgui.h>
#include "imgui_impl_vulkan.h"

// Imgui library implementation wrapper.
namespace gui {
class GUI {
public:
    // enabled - whether GUI is initialized and rendered
    // window - window the gui will be attached to
    // install_callbacks - whether to allow gui to set their own window callbacks
    // renderer - a renderer this gui is rendered with
    GUI(bool enabled, GLFWwindow* window, bool install_callbacks, renderer::Renderer& renderer);
    ~GUI();
    // non-copyable
    GUI(GUI const&) = delete;
    GUI operator=(GUI const&) = delete;
    bool IsEnabled() const;
    // GUI is allowed to be specified only after this call
    void BeginFrame() const;
    // GUI core final prepare for actual rendering commands.
    // GUI cannot be specified after this call.
    void PrepareForRendering() const;
    // Adds commands for gui rendering to given command bueffer
    void AppendCommandBuffer(VkCommandBuffer cmd_buffer) const;
private:
    // Initialize GLFW platform
    void InitPlatform(GLFWwindow* window, bool install_callbacks) const;
    // Init renderer binding
    void InitRenderer();
    // Renderer will not be changed in runtime therefore grab the reference
    renderer::Renderer& renderer_;
    const bool enabled_;
    std::unique_ptr<const renderer::vlk::DescriptorPool> descriptor_pool_;
    // TODO: This number should come from swapchain SelectImageCount() function
    const t::U16 min_image_count_ = 2;
};

}; // gui