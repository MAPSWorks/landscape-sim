//
// Created by Ivars Rusbergs in 2021
//
#include "lsim/renderer/vlk/command_buffer.h"

#include "lsim/base/log.h"
#include "lsim/renderer/vlk/command_pool.h"

namespace lsim::renderer::vlk {

CommandBuffer::CommandBuffer(const CommandPool &command_pool)
    : command_pool_(command_pool),
      command_buffer_(command_pool_.AllocateCommandBuffer(
          CommandPool::BufferLevel::kPrimary)) {

  base::Log::Info("renderer", "command buffer", "allocated");
}

const VkCommandBuffer &CommandBuffer::Handle() const { return command_buffer_; }

} // namespace lsim::renderer::vlk