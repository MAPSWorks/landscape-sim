#include "gui.h"
#include <base/log.h>
#include "imgui_impl_glfw.h"

namespace gui {
GUI::GUI(bool enabled, GLFWwindow* window, bool install_callbacks, renderer::Renderer& renderer) :
    renderer_(renderer),
    enabled_(enabled) {
    if (enabled_) {
        base::Log::Info("GUI: on");
        InitPlatform(window, install_callbacks);
        InitRenderer();
        base::Log::Info("GUI: GUI initialized");
    }
    else {
        base::Log::Info("GUI: off");
    }
}

GUI::~GUI() {
    if (enabled_) {
        base::Log::Info("GUI: GUI destroying destroying...");
        ImGui_ImplVulkan_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}

void GUI::InitPlatform(GLFWwindow* window, bool install_callbacks) const {
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    //ImGuiIO& io = ImGui::GetIO();
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    //ImGui::StyleColorsClassic();
    // Setup Platform bindings
    ImGui_ImplGlfw_InitForVulkan(window, true);
}

void GUI::InitRenderer() {
    ImGui_ImplVulkan_InitInfo init_info{};
    init_info.Instance = renderer_.GetContext().instance.Get();
    init_info.PhysicalDevice = renderer_.GetContext().device.GetGPU();
    init_info.Device = renderer_.GetContext().device.Get();
    init_info.QueueFamily = renderer_.GetContext().device.GetQueue().GetFamilyIndices().graphics.value();
    init_info.Queue = renderer_.GetContext().device.GetQueue().GetGraphics();
    init_info.PipelineCache = VK_NULL_HANDLE;
    // Descriptor pool
    const t::U32 descriptor_count = 1000;
    const std::vector<renderer::vlk::DescriptorPool::PoolSize> pool_sizes =
    {
        { renderer::vlk::DescriptorType::kSampler, descriptor_count },
        { renderer::vlk::DescriptorType::kCombinedImageSampler, descriptor_count },
        { renderer::vlk::DescriptorType::kSampledImage, descriptor_count },
        { renderer::vlk::DescriptorType::kStorageImage, descriptor_count },
        { renderer::vlk::DescriptorType::kUniformTexelBuffer, descriptor_count },
        { renderer::vlk::DescriptorType::kStorageTexelBuffer, descriptor_count },
        { renderer::vlk::DescriptorType::kUniformBuffer, descriptor_count },
        { renderer::vlk::DescriptorType::kStorageBuffer, descriptor_count },
        { renderer::vlk::DescriptorType::kUniformBufferDynamic, descriptor_count },
        { renderer::vlk::DescriptorType::kStorageBufferDynamic, descriptor_count },
        { renderer::vlk::DescriptorType::kInputAttachment, descriptor_count }
    };
    const t::U32 set_count = 1000;
    descriptor_pool_ = std::make_unique<const renderer::vlk::DescriptorPool>(renderer_.GetContext().device.Get(),
        pool_sizes, set_count * static_cast<t::U32>(pool_sizes.size()));
    init_info.DescriptorPool = descriptor_pool_->Get(); 
    init_info.Allocator = nullptr;
    // How many swapchain images we requested
    init_info.MinImageCount = min_image_count_; 
    // How many images swapchain actually created
    init_info.ImageCount = static_cast<t::U32>(renderer_.GetWindow().GetSwapchainObject().GetImageViews().size());
    init_info.CheckVkResultFn = renderer::vlk::ErrorCheck;
    // Render pass 
    VkRenderPass render_pass = renderer_.GetWindow().GetRenderPass();
    ImGui_ImplVulkan_Init(&init_info, render_pass);

    // Upload Fonts
    {
        /*
        VkResult err;
        // Use any command queue
        VkCommandPool command_pool = wd->Frames[wd->FrameIndex].CommandPool;
        VkCommandBuffer command_buffer = wd->Frames[wd->FrameIndex].CommandBuffer;

        err = vkResetCommandPool(renderer.GetContext().device.Get(), command_pool, 0);
        ErrCheck(err);
        VkCommandBufferBeginInfo begin_info = {};
        begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
        begin_info.flags |= VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
        err = vkBeginCommandBuffer(command_buffer, &begin_info);
        ErrCheck(err);
        */
        const renderer::vlk::CommandPool command_pool(renderer_.GetContext().device.Get(), renderer_.GetContext().device.GetQueue().GetFamilyIndices().graphics.value(),
            false, true) ;
        renderer::vlk::CommandBuffer command_buffer(command_pool);
        command_buffer.Begin(renderer::vlk::CommandBuffer::Usage::kOneTimeSubmit);
        ImGui_ImplVulkan_CreateFontsTexture(command_buffer.Get());
        command_buffer.End();
            
        /*
        VkSubmitInfo end_info = {};
        end_info.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
        end_info.commandBufferCount = 1;
        end_info.pCommandBuffers = &command_buffer;
        err = vkEndCommandBuffer(command_buffer);
        ErrCheck(err);
        err = vkQueueSubmit(renderer.GetContext().device.GetQueue().GetGraphics(), 1, &end_info, VK_NULL_HANDLE);
        ErrCheck(err);
        */
        renderer::vlk::ErrorCheck(vkDeviceWaitIdle(renderer_.GetContext().device.Get()));
        ImGui_ImplVulkan_DestroyFontUploadObjects();
    }
}

void GUI::Render() const {
    if (enabled_) {

    }
    /*
    bool show_demo_window = true;
    // Start the Dear ImGui frame
    ImGui_ImplVulkan_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
    ImGui::ShowDemoWindow(&show_demo_window);
    // Rendering
    ImGui::Render();
    memcpy(&wd->ClearValue.color.float32[0], &clear_color, 4 * sizeof(float));
    FrameRender(wd);

    FramePresent(wd);
    */
}

}; // gui