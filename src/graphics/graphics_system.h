#pragma once
#include <GLFW/glfw3.h>
#include <renderer/context.h>
#include "types.h"
#include "objects/triangle.h"

namespace graphics {
// System that takes renderable data as input and present images to screen as an output
class GraphicsSystem {
public:
    GraphicsSystem(const Settings &settings, GLFWwindow* window);
    ~GraphicsSystem();
    void RenderFrame() const;
    // Used to ensure nothing is currently in use on GPU
    void WaitForIdle() const;
private:
    const renderer::Context context_;
    const objects::Triangle triangle_;
};
}; // graphics