#pragma once
#include <GLFW/glfw3.h>
#include "context.h"
#include "vlk/swapchain.h"
#include "vlk/render_pass.h"
#include "vlk/framebuffers.h"

namespace renderer {
// Holds and manages objects that are dependant on swapchain
// Objects in this class are recreatable during the lifetime of the renderer
class Window {
public:
    Window(const Context& context,  GLFWwindow* window);
    ~Window();
    const vlk::Swapchain swapchain;
    const vlk::RenderPass render_pass;
    const vlk::Framebuffers framebuffers;
};
}; // renderer