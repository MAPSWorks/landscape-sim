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

void Test::Init() const {
  lsim::renderer::vlk::ShaderModule vertex_shader(renderer.GetDevice(),
                                                  "shaders/test.vert");

      // Graphics pipeline
      VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
  vertShaderStageInfo.sType =
      VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
  vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
}

} // namespace apps::test