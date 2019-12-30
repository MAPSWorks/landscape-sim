#include "gui.h"
#include <base/log.h>
#include <imgui/imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

namespace gui {
GUI::GUI(bool enabled, GLFWwindow* window, bool install_callbacks, renderer::Renderer& renderer) :
    renderer_(renderer),
    enabled_(enabled) {
    if (enabled_) {
        base::Log::Info("GUI: on");
        InitPlatform(window, install_callbacks);
        InitRenderer();
        UploadFonts();
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

// GLFW here should not interfere with application GLFW calls
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
    // Number is 'just to have enaugh'
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
    // Number is 'just to have enaugh'
    const t::U32 set_count = 1000;
    descriptor_pool_ = std::make_unique<const renderer::vlk::DescriptorPool>(renderer_.GetContext().device.Get(),
        pool_sizes, set_count * static_cast<t::U32>(pool_sizes.size()));
    init_info.DescriptorPool = descriptor_pool_->Get(); 
    init_info.Allocator = nullptr;
    // How many swapchain images we requested
    init_info.MinImageCount = renderer_.GetWindow().GetSwapchainObject().GetDesiredMinImageCount();
    // How many images swapchain actually created
    init_info.ImageCount = static_cast<t::U32>(renderer_.GetWindow().GetSwapchainObject().GetImageViews().size());
    init_info.CheckVkResultFn = renderer::vlk::ErrorCheck;
    // NOTE: This render pass is sued to create pipeline, so this gui should be rendered in the same pass.
    VkRenderPass render_pass = renderer_.GetWindow().GetRenderPass();
    // Initialize vulkan objects
    ImGui_ImplVulkan_Init(&init_info, render_pass);
}

// Create command buffer for single use to execute on graphics queue.
// Wait till it executes Vulkan GUI backend commands and free all objects.
void GUI::UploadFonts() const {
    const renderer::vlk::CommandPool command_pool(renderer_.GetContext().device.Get(), renderer_.GetContext().device.GetQueue().GetFamilyIndices().graphics.value(),
        false, true);
    renderer::vlk::CommandBuffer command_buffer(command_pool);
    command_buffer.Begin(renderer::vlk::CommandBuffer::Usage::kOneTimeSubmit);
    ImGui_ImplVulkan_CreateFontsTexture(command_buffer.Get());
    command_buffer.End();
    // Submit to graphics queue because graphics queue implicitly supports transfer
    renderer_.GetContext().device.GetQueue().Submit(renderer_.GetContext().device.GetQueue().GetGraphics(), command_buffer.Get());
    // Wait for upload then destroy
    renderer_.WaitForGPUIdle();
    ImGui_ImplVulkan_DestroyFontUploadObjects();
    // NOTE: command buffer is detroyed together with command pool.
}

bool GUI::IsEnabled() const {
    return enabled_;
}

void GUI::BeginFrame() const {
    if (enabled_) {
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
}

void GUI::PrepareForRendering() const {
    if (enabled_) {
        ImGui::Render();
    }
}

void GUI::AppendCommandBuffer(VkCommandBuffer cmd_buffer) const {
    if (enabled_) {
        ImGui_ImplVulkan_RenderDrawData(ImGui::GetDrawData(), cmd_buffer);
    }
}
}; // gui