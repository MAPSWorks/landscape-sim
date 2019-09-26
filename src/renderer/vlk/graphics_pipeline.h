#pragma once
#include "vulkan_shared.h"
#include "device.h"

namespace renderer::vlk {
// Graphics pipeline creation and storage facilities
// Pipeline is created passing structure of configurable data to constructor
class GraphicsPipeline {
public:
    // Graphical pipeline is created using create structure that is not tied to Vulkan
    // TODO: incomplete ... add things as needed ...
    struct ShaderStageParams {
        const std::string file_name;
        const ShaderStage stage;
        const std::string entry_point = "main";
    };
    struct VertexInputParamts {
        const std::vector<VkVertexInputBindingDescription> binding_descr;
        const std::vector<VkVertexInputAttributeDescription> attribute_descr;
    };
    enum class PrimitiveTopology {
        kTriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };
    struct InputAssemblyParams {
        const PrimitiveTopology primitive_topology = PrimitiveTopology::kTriangleList;
    };
    enum class PolygonMode {
        kFill = VK_POLYGON_MODE_FILL
    };
    enum class CullMode {
        kNone = VK_CULL_MODE_NONE,
        kBack = VK_CULL_MODE_BACK_BIT
    };
    struct RasterizationParams {
        const PolygonMode polygon_mode = PolygonMode::kFill;
        const CullMode cull_mode = CullMode::kNone;
    };
    struct MultisampleParams {
        const bool sample_shading_enable = false;
    };
    struct DepthStencilParams {
        // TODO ...
    };
    struct ColorBlendParams {
        const bool blend_enable = false;
    };
    // Pipeline layout configuration
    struct LayoutParams {
        // Describes descriptors that are to be used in pipeline
        // Resources with handles VkDescriptorSetLayout should stick around while pipeline is used
        const std::vector<VkDescriptorSetLayout> layouts;
        // ... TODO push constants here
    };
    // Actual parameter structure
    struct CreateParams {
        // Name of a pipeline (text in free form)
        std::string name;
        // programmable stage
        const std::vector<ShaderStageParams> shader_stages;
        const VertexInputParamts vertex_input;
        // Fixed stage
        const InputAssemblyParams input_assembly;
        const RasterizationParams rasterization;
        const MultisampleParams multisample;
        // TODO: depth stencil params
        const ColorBlendParams color_blend;
        // Descriptor sets and pus constants
        const LayoutParams layout;
    };

    // TODO: give pipeline some name through parameter?
    GraphicsPipeline(const VkDevice& device, const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent,  
        const CreateParams& create_params);
    ~GraphicsPipeline();
    GraphicsPipeline(GraphicsPipeline const&) = delete;
    GraphicsPipeline operator=(GraphicsPipeline const&) = delete;
    const VkPipeline& Get() const;
    const std::string& GetName() const;
    const CreateParams& GetCreateParams() const;
private:
    VkPipelineLayout CreatePipelineLayout(const LayoutParams& params) const;
    VkPipeline Create(const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent, 
        const CreateParams& create_params) const;
    // Reference to resource this renderable is created with
    const VkDevice& device_;
    // Parameters with whitch this pipeline was created
    const CreateParams create_params_;
    // Name of pipeline in free form
    const std::string name_;
    const VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
    const VkPipeline pipeline_ = VK_NULL_HANDLE;
};
};  // renderer vlk