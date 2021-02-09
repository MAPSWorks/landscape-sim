//
// Created by Ivars Rusbergs in 2021
//
#include "test.h"

#include <vulkan/vulkan.h>

#include <lsim/base/log.h>
#include <lsim/platform/i_application.h>
#include <lsim/platform/types.h>
#include <lsim/renderer/vlk/shader_module.h>

namespace apps::test {
// TODO: should come from elsewhere
lsim::platform::Settings user_settings{"Alpha app", 1,
                                       lsim::Size<uint32_t>(1280, 720)};

Test::Test(int argc, char *argv[])
    : lsim::platform::IApplication(argc, argv, user_settings) {
  Init();
  lsim::base::Log::Info("test application", "initialized");
}

void Test::Init() {
  // Note: shader modules can be destroyed after pipeline creation
  lsim::renderer::vlk::ShaderModule vertex_shader(
      renderer_.GetDeviceObject().Get(), "shaders/test.vert.spv");
  lsim::renderer::vlk::ShaderModule fragment_shader(
      renderer_.GetDeviceObject().Get(), "shaders/test.frag.spv");
  // Graphics pipeline
  // Shader stage
  VkPipelineShaderStageCreateInfo vert_stage_create_info{};
  vert_stage_create_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vert_stage_create_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
  vert_stage_create_info.module = vertex_shader.Get();
  vert_stage_create_info.pName = "main";
  // vert_stage_create_info.pSpecializationInfo  - can adjust shader behaviour
  VkPipelineShaderStageCreateInfo frag_stage_create_info{};
  frag_stage_create_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  frag_stage_create_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
  frag_stage_create_info.module = fragment_shader.Get();
  frag_stage_create_info.pName = "main";

  VkPipelineShaderStageCreateInfo shader_stages[] = {vert_stage_create_info,
                                                     frag_stage_create_info};
  (void)shader_stages;

  // vertex input
  VkPipelineVertexInputStateCreateInfo vertex_input_info{};
  vertex_input_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vertex_input_info.vertexBindingDescriptionCount = 0;
  vertex_input_info.pVertexBindingDescriptions = nullptr; // Optional
  vertex_input_info.vertexAttributeDescriptionCount = 0;
  vertex_input_info.pVertexAttributeDescriptions = nullptr; // Optional

  // input assembly
  VkPipelineInputAssemblyStateCreateInfo input_assembly_info{};
  input_assembly_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_assembly_info.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
  input_assembly_info.primitiveRestartEnable =
      VK_FALSE; // cool thing, can break triangle strips

  // viewport and scissors
  VkViewport viewport{};
  viewport.x = 0.0f;
  viewport.y = 0.0f;

  // should be swapchain extent
  viewport.width = static_cast<float>(renderer_.GetSwapchinObject().GetExtent().width);
  viewport.height = static_cast<float>(renderer_.GetSwapchinObject().GetExtent().height);
  viewport.minDepth = 0.0f;
  viewport.maxDepth = 1.0f;
  VkRect2D scissor{};

  // swapchain extent
  scissor.offset = {0, 0};
  scissor.extent = renderer_.GetSwapchinObject().GetExtent();
  VkPipelineViewportStateCreateInfo viewport_states_info{};
  viewport_states_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_states_info.viewportCount = 1;
  viewport_states_info.pViewports = &viewport;
  viewport_states_info.scissorCount = 1;
  viewport_states_info.pScissors = &scissor;

  // Rasterizer
  VkPipelineRasterizationStateCreateInfo raster_state_info{};
  raster_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  raster_state_info.depthClampEnable = VK_FALSE;
  raster_state_info.rasterizerDiscardEnable = VK_FALSE;
  raster_state_info.polygonMode = VK_POLYGON_MODE_FILL;
  raster_state_info.lineWidth = 1.0f;
  raster_state_info.cullMode = VK_CULL_MODE_BACK_BIT;
  raster_state_info.frontFace = VK_FRONT_FACE_CLOCKWISE;
  raster_state_info.depthBiasEnable = VK_FALSE;
  raster_state_info.depthBiasConstantFactor = 0.0f; // Optional
  raster_state_info.depthBiasClamp = 0.0f;          // Optional
  raster_state_info.depthBiasSlopeFactor = 0.0f;    // Optional

  // Multisample
  VkPipelineMultisampleStateCreateInfo multisample_info{};
  multisample_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  multisample_info.sampleShadingEnable = VK_FALSE;
  multisample_info.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
  multisample_info.minSampleShading = 1.0f;          // Optional
  multisample_info.pSampleMask = nullptr;            // Optional
  multisample_info.alphaToCoverageEnable = VK_FALSE; // Optional
  multisample_info.alphaToOneEnable = VK_FALSE;      // Optional

  // Depth stencil
  // ...

  // Color blending
  VkPipelineColorBlendAttachmentState color_blend_attachment{};
  color_blend_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  color_blend_attachment.blendEnable = VK_FALSE;
  color_blend_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  color_blend_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.colorBlendOp = VK_BLEND_OP_ADD;             // Optional
  color_blend_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;  // Optional
  color_blend_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO; // Optional
  color_blend_attachment.alphaBlendOp = VK_BLEND_OP_ADD;             // Optional
  VkPipelineColorBlendStateCreateInfo color_blend_info{};
  color_blend_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  color_blend_info.logicOpEnable = VK_FALSE;
  color_blend_info.logicOp = VK_LOGIC_OP_COPY; // Optional
  color_blend_info.attachmentCount = 1;
  color_blend_info.pAttachments = &color_blend_attachment;
  color_blend_info.blendConstants[0] = 0.0f; // Optional
  color_blend_info.blendConstants[1] = 0.0f; // Optional
  color_blend_info.blendConstants[2] = 0.0f; // Optional
  color_blend_info.blendConstants[3] = 0.0f; // Optional
  // Pipeline layout
  VkPipelineLayoutCreateInfo layout_create_info{};
  layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layout_create_info.setLayoutCount = 0;            // Optional
  layout_create_info.pSetLayouts = nullptr;         // Optional
  layout_create_info.pushConstantRangeCount = 0;    // Optional
  layout_create_info.pPushConstantRanges = nullptr; // Optional
  layout_ = std::make_unique<lsim::renderer::vlk::PipelineLayout>(
      renderer_.GetDeviceObject().Get(), layout_create_info);
  // Render pass
  render_pass_ = std::make_unique<lsim::renderer::vlk::RenderPass>(
      renderer_.GetDeviceObject().Get(),
      renderer_.GetSwapchinObject().GetSurfaceFormat().format, VkFormat{});
}

} // namespace apps::test