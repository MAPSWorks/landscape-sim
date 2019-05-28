#include "graphics_pipeline.h"
#include <deque>
#include <base/log.h>
#include "shader_module.h"

namespace renderer::vlk {
GraphicsPipeline::GraphicsPipeline(const VkDevice& device, const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent,
    const CreateParams& create_params) :
    device_(device),
    create_params_(create_params),
    name_(create_params.name),
    pipeline_layout_(CreatePipelineLayout(create_params.layout)),
    pipeline_(Create(render_pass, swapchain_extent, create_params)) {
    base::Log::Info("Renderer: graphics pipeline '", name_, "' created");
}

GraphicsPipeline::~GraphicsPipeline() {
    base::Log::Info("Renderer: graphics pipeline '", name_, "' destroying...");
    vkDestroyPipeline(device_, pipeline_, nullptr);
    vkDestroyPipelineLayout(device_, pipeline_layout_, nullptr);
}

const VkPipeline& GraphicsPipeline::Get() const {
    return pipeline_;
}

const std::string& GraphicsPipeline::GetName() const {
    return name_;
}

const GraphicsPipeline::CreateParams& GraphicsPipeline::GetCreateParams() const {
    return create_params_;
}

VkPipelineLayout GraphicsPipeline::CreatePipelineLayout(const LayoutParams& params) const {
    VkPipelineLayoutCreateInfo pipeline_layout_create_info {};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipeline_layout_create_info.setLayoutCount = params.layout_count; // Optional
    pipeline_layout_create_info.pSetLayouts = nullptr; // Optional
    pipeline_layout_create_info.pushConstantRangeCount = 0; // Optional
    pipeline_layout_create_info.pPushConstantRanges = nullptr; // Optional
    VkPipelineLayout pipeline_layout;
    ErrorCheck(vkCreatePipelineLayout(device_, &pipeline_layout_create_info, nullptr, &pipeline_layout));
    base::Log::Info("Renderer: pipeline '", name_, "' pipeline layout created, count - ",
        pipeline_layout_create_info.setLayoutCount);
    return pipeline_layout;
}

// Create pipeline from given outside configurable parameters
VkPipeline GraphicsPipeline::Create(const VkRenderPass& render_pass, const VkExtent2D& swapchain_extent, 
    const CreateParams& create_params) const {
    // Programmamble part
    std::vector<VkPipelineShaderStageCreateInfo> stage_create_infos;
    // Shader modules are OK to be destroyed only after pipeline is created
    // That's why we make sure that modules are not created inside any brackets
    // NOTE: Deque is used here instead of vector because vector requires object to be copy-constructable,
    // but this class does not have it
    std::deque<ShaderModule> shader_modules;
    for (const auto& stage_params : create_params.shader_stages) {
        shader_modules.emplace_back(device_, stage_params.file_name);
        VkPipelineShaderStageCreateInfo stage_create_info {};
        stage_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        stage_create_info.stage = static_cast<VkShaderStageFlagBits>(stage_params.stage);
        stage_create_info.module = shader_modules.back().Get();
        stage_create_info.pName = stage_params.entry_point.c_str();
        stage_create_infos.push_back(stage_create_info);
        base::Log::Info("Renderer: pipeline '", name_, "' ", ToString(stage_create_info.stage),
                    " shader stage created. Entry point - ", stage_create_info.pName);
    }
    // Fixed-function part
    // Vertex input
    VkPipelineVertexInputStateCreateInfo vertex_input_create_info {};
    vertex_input_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
    vertex_input_create_info.vertexBindingDescriptionCount = 0;
    vertex_input_create_info.pVertexBindingDescriptions = nullptr; // Optional
    vertex_input_create_info.vertexAttributeDescriptionCount = 0;
    vertex_input_create_info.pVertexAttributeDescriptions = nullptr; // Optional
    // Input assambly
    VkPipelineInputAssemblyStateCreateInfo input_assembly_create_info {};
    input_assembly_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
    input_assembly_create_info.topology = static_cast<VkPrimitiveTopology>(create_params.input_assembly.primitive_topology);
    input_assembly_create_info.primitiveRestartEnable = VK_FALSE;
    base::Log::Info("Renderer: pipeline '", name_, "' primitive topology set - ",
        ToString(input_assembly_create_info.topology));
    // Viewport and scissors
    // Potentially configurble, but usually the same for all
    VkViewport viewport {};
    viewport.x = 0.0f;
    viewport.y = 0.0f;
    viewport.width = static_cast<float>(swapchain_extent.width);
    viewport.height = static_cast<float>(swapchain_extent.height);
    viewport.minDepth = 0.0f;
    viewport.maxDepth = 1.0f;
    VkRect2D scissor  {};
    scissor.offset = { 0, 0 };
    scissor.extent = { swapchain_extent.width, swapchain_extent.height };
    VkPipelineViewportStateCreateInfo viewport_states_create_info {};
    viewport_states_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewport_states_create_info.viewportCount = 1;
    viewport_states_create_info.pViewports = &viewport;
    viewport_states_create_info.scissorCount = 1;
    viewport_states_create_info.pScissors = &scissor;
    base::Log::Info("Renderer: pipeline '", name_, "' viewport set - (",
        viewport.width, "," , viewport.height,")");
    // Rasterizer
    VkPipelineRasterizationStateCreateInfo raster_state_create_info {};
    raster_state_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
    raster_state_create_info.depthClampEnable = VK_FALSE;
    raster_state_create_info.rasterizerDiscardEnable = VK_FALSE;
    raster_state_create_info.polygonMode = static_cast<VkPolygonMode>(create_params.rasterization.polygon_mode);
    raster_state_create_info.lineWidth = 1.0f;
    raster_state_create_info.cullMode = static_cast<VkCullModeFlagBits>(create_params.rasterization.cull_mode);
    raster_state_create_info.frontFace = VK_FRONT_FACE_CLOCKWISE;;
    raster_state_create_info.depthBiasEnable = VK_FALSE;
    raster_state_create_info.depthBiasConstantFactor = 0.0f; // Optional
    raster_state_create_info.depthBiasClamp = 0.0f; // Optional
    raster_state_create_info.depthBiasSlopeFactor = 0.0f; // Optional
    // In structure cull mode type is flags(uint32_t) but our parameter is bit value
    base::Log::Info("Renderer: pipeline '", name_, "' rasterization set - ",
        ToString(raster_state_create_info.polygonMode), " ", 
        ToString(static_cast<VkCullModeFlagBits>(raster_state_create_info.cullMode)));
    // Multisample
    VkPipelineMultisampleStateCreateInfo multisample_create_info {};
    multisample_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
    multisample_create_info.sampleShadingEnable = static_cast<VkBool32>(create_params.multisample.sample_shading_enable);
    multisample_create_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
    multisample_create_info.minSampleShading = 1.0f; // Optional
    multisample_create_info.pSampleMask = nullptr; // Optional
    multisample_create_info.alphaToCoverageEnable = VK_FALSE; // Optional
    multisample_create_info.alphaToOneEnable = VK_FALSE; // Optional
    base::Log::Info("Renderer: pipeline '", name_, "' multisample enabled - ",
        ToString(create_params.multisample.sample_shading_enable));
    // Depth stencil
    // ....
    // Color blending
    VkPipelineColorBlendAttachmentState color_blend_attachment {};
    color_blend_attachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | 
        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
    color_blend_attachment.blendEnable = static_cast<VkBool32>(create_params.color_blend.blend_enable);
    color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD; // Optional
    color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE; // Optional
    color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
    color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD; // Optional
    VkPipelineColorBlendStateCreateInfo color_blend_create_info {};
    color_blend_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    color_blend_create_info.logicOpEnable = VK_FALSE;
    color_blend_create_info.logicOp = VK_LOGIC_OP_COPY; // Optional
    color_blend_create_info.attachmentCount = 1;
    color_blend_create_info.pAttachments = &color_blend_attachment;
    color_blend_create_info.blendConstants[0] = 0.0f; // Optional
    color_blend_create_info.blendConstants[1] = 0.0f; // Optional
    color_blend_create_info.blendConstants[2] = 0.0f; // Optional
    color_blend_create_info.blendConstants[3] = 0.0f; // Optional
    base::Log::Info("Renderer: pipeline '", name_, "' color blend enabled - ",
        ToString(create_params.color_blend.blend_enable)); 

    // The pipeline itself
    VkGraphicsPipelineCreateInfo create_info {};
    create_info.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    create_info.stageCount = static_cast<uint32_t>(stage_create_infos.size());
    create_info.pStages = stage_create_infos.data();
    create_info.pVertexInputState = &vertex_input_create_info;
    create_info.pInputAssemblyState = &input_assembly_create_info;
    create_info.pViewportState = &viewport_states_create_info;
    create_info.pRasterizationState = &raster_state_create_info;
    create_info.pMultisampleState = &multisample_create_info;
    // TODO: complete later
    create_info.pDepthStencilState = nullptr; 
    create_info.pColorBlendState = &color_blend_create_info;
    // TODO: some state can be modified without recreating pipeline check : VkPipelineDynamicStateCreateInfo
    create_info.pDynamicState = nullptr; // Optional
    create_info.layout = pipeline_layout_;
    create_info.renderPass = render_pass;
    // Subpass index
    create_info.subpass = 0;
    // Pipelines can be derived to gain speed, VK_PIPELINE_CREATE_DERIVATIVE_BIT. must be set in flags
    create_info.basePipelineHandle = nullptr; // Optional
    create_info.basePipelineIndex = -1; // Optional
    VkPipeline pipeline;
    ErrorCheck(vkCreateGraphicsPipelines(device_, VK_NULL_HANDLE, 1, &create_info, nullptr, &pipeline));
    return pipeline;
}


};  // renderer vlk