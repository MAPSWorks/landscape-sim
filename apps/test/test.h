//
// Created by Ivars Rusbergs in 2021
//
// Application used as an experimental app for engine testing
#ifndef LSIM_APPS_TEST_H_
#define LSIM_APPS_TEST_H_
#include <memory>

#include <lsim/platform/i_application.h>
#include <lsim/renderer/vlk/pipeline_graphics.h>
#include <lsim/renderer/vlk/pipeline_layout.h>
#include <lsim/renderer/vlk/render_pass.h>

namespace apps::test {
class Test : public lsim::platform::IApplication {
public:
  Test(int argc, char *argv[]);

private:
  void InitPipeline();
  std::unique_ptr<lsim::renderer::vlk::PipelineLayout> layout_;
  std::unique_ptr<lsim::renderer::vlk::RenderPass> render_pass_;
  std::unique_ptr<lsim::renderer::vlk::PipelineGraphics> pipeline_;
};
} // namespace apps::test

#endif