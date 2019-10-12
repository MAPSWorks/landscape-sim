#pragma once
#include <vector>
#include <memory>
#include "context.h"
#include "vlk/swapchain.h"
#include "vlk/render_pass.h"

// Holds and manages objects that are dependant on swapchain.
// Sensitive to window resize.
// Objects in this class are recreatable during the lifetime of the renderer.
namespace renderer {
class Window {
public:
    Window(const Context& context);
    const VkSwapchainKHR& GetSwapchain() const;
    // Get wrapper renderable
    const vlk::Swapchain& GetSwapchainObject() const;
    const VkRenderPass& GetRenderPass() const;
    // Current aspect ratio of the window
    t::F32 GetAspectRatio() const;
    // Delete and recreate swapchain and all objects depending on swapchine format
    void RecreateSwapchain();
private:
    // Creates also swapchain dependant objects of this class
    void CreateSwapchain();
    // Reference to the context this class is used with
    const Context& context_;
    std::unique_ptr<vlk::Swapchain> swapchain_;
    std::unique_ptr<vlk::RenderPass> render_pass_;
};
}; // renderer