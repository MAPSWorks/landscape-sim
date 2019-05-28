#pragma once
#include <GLFW/glfw3.h>
#include "vlk/instance.h"
#include "vlk/surface.h"
#include "vlk/device.h"

// Context - the circumstances that form the setting for an event, statement, 
//           or idea, and in terms of which it can be fully understood.
//                                                               - Wikipedia
//
// System-wide global state of rendering
// Must be initialized first for any rendering system
// Objects here are guarteed to not change during the lifetime of renderer instance
// TODO: use struct if context remains POD
namespace renderer {
class Context {
public:
    Context(const vlk::Settings& settings, GLFWwindow* window);
    ~Context();
    // Store the pointer to window handle
    GLFWwindow* window_glfw;
    // System-wide objects
    const vlk::Instance instance;
    const vlk::Surface surface;
    const vlk::Device device;
};
}; // renderer