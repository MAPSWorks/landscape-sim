//
// Created by Ivars Rusbergs in 2021
//
// Application used as an experimental app
#ifndef LSIM_APPS_TEST_H_
#define LSIM_APPS_TEST_H_
#include <memory>
#include <vector>

#include <lsim/platform/i_application.h>
#include <lsim/renderer/vlk/command_buffer.h>
#include <lsim/renderer/vlk/command_pool.h>
#include <lsim/renderer/vlk/framebuffer.h>
#include <lsim/renderer/vlk/pipeline_graphics.h>
#include <lsim/renderer/vlk/pipeline_layout.h>
#include <lsim/renderer/vlk/render_pass.h>
#include <lsim/renderer/vlk/semaphore.h>
#include <lsim/renderer/vlk/fence.h>

namespace apps::test {

const int kFramesInFlight = 2;

struct FrameResource {
  explicit FrameResource(VkDevice device);

  lsim::renderer::vlk::Semaphore sem_image_available;
  lsim::renderer::vlk::Semaphore sem_render_finished;
  lsim::renderer::vlk::Fence fence_drawing_finished;
};



class Test : public lsim::platform::IApplication {
public:
  Test(int argc, char **argv);

private:
  void RenderFrame() override;
  void OnExit() override;
  void InitPipeline();
  void CreateFramebuffers();
  void CreateCommandBuffers();
  void CreateFrameResources();
  std::unique_ptr<lsim::renderer::vlk::PipelineLayout> layout_;
  std::unique_ptr<lsim::renderer::vlk::RenderPass> render_pass_;
  std::unique_ptr<lsim::renderer::vlk::PipelineGraphics> pipeline_;
  // List of framebuffer for each image in swapchain
  std::vector<lsim::renderer::vlk::Framebuffer> framebuffers_;
  std::unique_ptr<lsim::renderer::vlk::CommandPool> command_pool_;
  std::vector<lsim::renderer::vlk::CommandBuffer> command_buffers_;

  // Frames in flight
  uint16_t current_frame_ = 0;
  std::vector<FrameResource> frame_resources_;
};
} // namespace apps::test

#endif