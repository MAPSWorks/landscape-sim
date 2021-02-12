//
// Created by Ivars Rusbergs in 2021
//
// Application used as an experimental app for engine testing
#ifndef LSIM_APPS_TEST_H_
#define LSIM_APPS_TEST_H_
#include <memory>
#include <vector>

#include <lsim/platform/i_application.h>
#include <lsim/renderer/vlk/pipeline_graphics.h>
#include <lsim/renderer/vlk/pipeline_layout.h>
#include <lsim/renderer/vlk/render_pass.h>
#include <lsim/renderer/vlk/framebuffer.h>
#include <lsim/renderer/vlk/command_pool.h>


namespace apps::test {
class Test : public lsim::platform::IApplication {
public:
  Test(int argc, char *argv[]);

private:
  virtual void RenderFrame() const override;
  void InitPipeline();
  void CreateFramebuffers();
  void CreateCommandBuffers();
  std::unique_ptr<lsim::renderer::vlk::PipelineLayout> layout_;
  std::unique_ptr<lsim::renderer::vlk::RenderPass> render_pass_;
  std::unique_ptr<lsim::renderer::vlk::PipelineGraphics> pipeline_;
  // List of framebuffer for each image in swapchain
  std::vector<lsim::renderer::vlk::Framebuffer> framebuffers_;
  std::unique_ptr<lsim::renderer::vlk::CommandPool> command_pool_;
};
} // namespace apps::test

#endif