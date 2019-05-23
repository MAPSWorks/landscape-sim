#include "semaphore.h"
#include <base/log.h>

namespace renderer::vlk {
Semaphore::Semaphore(const VkDevice& device) : 
    device_(device),
    semaphore_(Create()) {
    base::Log::Info("Renderer: semaphore created");
}

Semaphore::~Semaphore() {
    base::Log::Info("Renderer: semaphore destroying...");
    vkDestroySemaphore(device_, semaphore_, nullptr);
}

const VkSemaphore& Semaphore::Get() const {
    return semaphore_;
}

VkSemaphore Semaphore::Create() const {
    VkSemaphoreCreateInfo create_info = {};
    create_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
    VkSemaphore semaphore;
    ErrorCheck(vkCreateSemaphore(device_, &create_info, nullptr, &semaphore));
    return semaphore;
}
};// renderer vlk