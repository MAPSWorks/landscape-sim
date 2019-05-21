#pragma once
#include <GLFW/glfw3.h>
#include "vlk/instance.h"
#include "vlk/surface.h"
#include "vlk/device.h"
#include "vlk/swapchain.h"

namespace renderer {
// Context - the circumstances that form the setting for an event, statement, 
//           or idea, and in terms of which it can be fully understood.
//                                                               - Wikipedia
//
// System-wide global state of rendering
// Must be initialized first for any rendering system
// TODO: use struct if context remains POD
class Context {
public:
    Context(const vlk::Settings& settings, GLFWwindow* window);
    ~Context();
    // System-wide objects
    const vlk::Instance instance;
    const vlk::Surface surface;
    const vlk::Device device;
    const vlk::Swapchain swapchain;
//private:
};
}; // renderer