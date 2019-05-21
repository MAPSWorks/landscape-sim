#pragma once
#include <GLFW/glfw3.h>
#include "vlk/types.h"
#include "context.h"

namespace renderer {
// General rendering class used to set-up means to render and render data in
// it's pure form (without knowing details of what is being rendered)
class Renderer {
public:
    Renderer(vlk::Settings settings, GLFWwindow* window);
    ~Renderer();
    const Context& GetContext() const;
    void RenderFrame() const;
    // Used to ensure nothing is currently in use on GPU
    void WaitForGPUIdle() const;
private:
    const Context context_;


};
}; // renderer