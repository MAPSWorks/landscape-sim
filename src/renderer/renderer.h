#pragma once
#include <GLFW/glfw3.h>
#include <base/types.h>
#include <base/json_loader.h>
#include "context.h"
#include "window.h"
#include "descriptor_set_layout_cache.h"
#include "pipeline_manager.h"
#include "frame_resource.h"
#include "frame_manager.h"
#include "vlk/memory_allocator.h"

// General rendering class used to set-up means to render and render data in
// it's pure form (without knowing details of what is being rendered)
namespace renderer {
class Renderer {
public:
    Renderer(const base::JSONLoader& setting_loader, GLFWwindow* window);
    const Context& GetContext() const;
    const Window& GetWindow() const;
    DescriptorSetLayoutCache& GetDescriptorSetLayoutCache();
    PipelineManager& GetPipelineManager();
    // Used to ensure nothing is currently in use on GPU
    void WaitForGPUIdle() const;
    // Window is resized, new size is passed through parameter
    void Resize();
    // Record command buffer that is currently processed by frame manager
    // Recording should happen between following calls
    void BeginRecordCurrentCommandBuffer();
    void EndRecordCurrentCommandBuffer();
    // Get command buffer from currently CPU processed resources
    const vlk::CommandBuffer& GetCurrentCommandBuffer();
    // Frame is rendered between these calls
    void FrameBegin();
    void FrameEnd();
    // Access memory allocator renderable for buffer creation
    const vlk::MemoryAllocator& GetMemoryAllocator() const;
private:
    // Constant throughout the life of the renderer
    const Context context_;
    // Changed upon window resize (dependant on window siize)
    Window window_;
    DescriptorSetLayoutCache descriptor_set_layout_cache_;
    PipelineManager pipeline_manager_;
    FrameManager frame_manager_;
    // Keeps track and manages buffer memory allocations
    vlk::MemoryAllocator memory_allocator_;
};
}; // renderer