#pragma once
#include <memory>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <renderer/renderer.h>

// Imgui library implementation wrapper.
// It initializes GLFW callbacks and Vulkan objects necessery to render GUI.
// Prepares frame for GUI rendering and adds rendering commands to given command buffer.
// Actual GUI sepcification is done in each indivdual entity that has gui and it is called
// after GUI frame is prepared with function NewFrame.
// NOTE: in case of problems check synchronization.
// GLFW and Vulkan backends are used almost unmodified from Dear Imgui example.
// But not all functionality is used from those files.
// NOTE: to get a full control of Vulkan backend implement Sacha Willems imgui example.
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
    void UploadFonts() const;
    // Renderer will not be changed in runtime therefore grab the reference
    renderer::Renderer& renderer_;
    // NOTE: Enable cannot be changed during applicaion run time!
    const bool enabled_;
    // Used to allocate descriptor sets for GUI
    std::unique_ptr<const renderer::vlk::DescriptorPool> descriptor_pool_;
};
}; // gui