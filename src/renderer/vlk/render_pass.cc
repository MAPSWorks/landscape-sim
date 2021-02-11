//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/render_pass.h"

#include <vector>

#include <vulkan/vulkan.h>

#include "lsim/base/log.h"
#include "vulkan_shared.h"

namespace lsim::renderer::vlk {
RenderPass::RenderPass(const VkDevice &device, const VkFormat &swapchain_format,
                       const VkFormat &depth_format)
    : device_(device), render_pass_(Create(swapchain_format, depth_format)) {
  base::Log::Info("renderer", "render pass", "created");
}

RenderPass::~RenderPass() {
  base::Log::Info("renderer", "render pass", "destroying..");
  vkDestroyRenderPass(device_, render_pass_, nullptr);
}

const VkRenderPass &RenderPass::Get() const { return render_pass_; }

// Defines how many color and depth buffers there will be, how many samples to
// use for each of them and how their contents should be handled throughout the
// rendering operations
VkRenderPass RenderPass::Create(const VkFormat &swapchain_format,
                                const VkFormat &depth_format) const {

  const auto attachments = DescribeAttachments(swapchain_format, depth_format);
  // Attachment reference
  VkAttachmentReference color_attachment_ref{};
  // Specifies which attachment to reference by its index in the attachment
  // descriptions array
  // This index should not be hardcoded (probably) but recieved from
  // attachment array. It is referenced in fragment shader.
  color_attachment_ref.attachment = 0;
  color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
  /*
  VkAttachmentReference depth_attachment_ref{};
  depth_attachment_ref.attachment = 1;
  depth_attachment_ref.layout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
*/
  // Subpasses
  VkSubpassDescription subpass{};
  subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  // The index of the attachment in this array is directly referenced from the
  // fragment shader with the layout(location = 0) out vec4 outColor directive!
  subpass.pColorAttachments = &color_attachment_ref;
  // subpass.pDepthStencilAttachment = &depth_attachment_ref;
  // Subpass dependancies
  // Specify memory and execution dependencies between subpasses
  /*
  VkSubpassDependency dependency = {};
  dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass = 0;
  dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_READ_BIT |
                             VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
                             */
  // Pass
  VkRenderPassCreateInfo render_pass_create_info{};
  render_pass_create_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  render_pass_create_info.attachmentCount =
      static_cast<uint32_t>(attachments.size());
  render_pass_create_info.pAttachments = attachments.data();
  render_pass_create_info.subpassCount = 1;
  render_pass_create_info.pSubpasses = &subpass;
  // render_pass_create_info.dependencyCount = 1;
  // render_pass_create_info.pDependencies = &dependency;
  VkRenderPass render_pass;
  ErrorCheck(vkCreateRenderPass(device_, &render_pass_create_info, nullptr,
                                &render_pass));
  return render_pass;
}

// Attachment is Vulkan’s name for what you might otherwise know as a render
// target - an Image to be used as output from rendering.You don’t point to
// specific Images here – you just describe their formats. (Understanding
// Vulkan objects) Actual images to render to are defined in framebuffers to
// which this renderpass is compatable Single color buffer attachmnt
std::vector<VkAttachmentDescription>
RenderPass::DescribeAttachments(const VkFormat &swapchain_format,
                                const VkFormat &depth_format) const {

  VkAttachmentDescription color_attachment{};
  // The format of the color attachment should match the format of the swap
  // chain images because we are rendering to it
  color_attachment.format = swapchain_format;
  // No multisampling
  color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  // What to do with data in attachment before rendering (color and depth data)
  color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  // After rendering
  color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
  // Stencil data
  color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  // Images to be presented in the swap chain
  color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  // Depth attachment
  VkAttachmentDescription depth_attachment{};
  depth_attachment.format = depth_format;
  depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
  depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
  depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
  depth_attachment.finalLayout =
      VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
  (void)depth_attachment;
  // The index of the attachment in this array is directly referenced from the
  // fragment shader with the layout(location = 0) out vec4 outColor directive!
  // TODO: attachments should be added somehow dynamically to control their
  // index in array
  std::vector<VkAttachmentDescription> attachments = {color_attachment /*,
                                                      depth_attachment*/};
  return attachments;
}

} // namespace lsim::renderer::vlk