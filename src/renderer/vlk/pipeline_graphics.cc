//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/pipeline_graphics.h"

#include "lsim/base/log.h"

namespace lsim::renderer::vlk {
PipelineGraphics::PipelineGraphics(const VkDevice &device)
    : device_(device), pipeline_(Create()) {
  base::Log::Info("renderer", "pipeline", "created");
}

PipelineGraphics::~PipelineGraphics() {
  base::Log::Info("renderer", "pipeline", "destroying..");
}

const VkPipeline &PipelineGraphics::Get() const { return pipeline_; }
} // namespace lsim::renderer::vlk