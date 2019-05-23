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
    // Redefine vulkan values to decouple Vulkan and context where we use this library
    enum class ShaderStage {
        kVertex = VK_SHADER_STAGE_VERTEX_BIT,
        kFragment = VK_SHADER_STAGE_FRAGMENT_BIT
    };
    struct ShaderStageParams {
        const std::string file_name;
        const ShaderStage stage;
        const std::string entry_point = "main";
    };
    struct VertexInputParamts {
        // TODO: ...
    };
    enum class PrimitiveTopology {
        kTriangleList = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST
    };
    struct InputAssemblyParams {
        const PrimitiveTopology primitive_topology = PrimitiveTopology::kTriangleList;
    };
    struct ViewportParams {
        const uint32_t width;
        const uint32_t height;
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
        const uint32_t layout_count = 0;
    };
    // Actual parameter structure
    struct CreateParams {
        // Name of a pipeline (text in free form)
        std::string name;
        // programmable stage
        const std::vector<ShaderStageParams> shader_stages;
        // Fixed stage
        // TODO: vertex input stage here ... VertexInputParamts
        const InputAssemblyParams input_assembly;
        const ViewportParams viewport;
        const RasterizationParams rasterization;
        const MultisampleParams multisample;
        // TODO: depth stencil params
        const ColorBlendParams color_blend;
        // TODO: when pipeline layout is moved out, this param should be removed
        const LayoutParams layout;
    };

    // TODO: give pipeline some name through parameter?
    GraphicsPipeline(const VkDevice& device, const VkRenderPass& render_pass, const CreateParams& create_params);
    ~GraphicsPipeline();
    GraphicsPipeline(GraphicsPipeline const&) = delete;
    GraphicsPipeline operator=(GraphicsPipeline const&) = delete;
    const VkPipeline& Get() const;
    const std::string& GetName() const;
private:
    VkPipelineLayout CreatePipelineLayout(const LayoutParams& params) const;
    VkPipeline Create(const VkRenderPass& render_pass, const CreateParams& create_params) const;
    // Reference to resource this object is created with
    const VkDevice& device_;
    // Name of pipeline in free form
    const std::string name_;
    const VkPipelineLayout pipeline_layout_ = VK_NULL_HANDLE;
    const VkPipeline pipeline_ = VK_NULL_HANDLE;
};
};  // renderer vlk