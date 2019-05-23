#include "sync_manager.h"
#include <vulkan/vulkan.h>
#include <base/log.h>

namespace renderer {
SyncManager::SyncManager(const VkDevice& device) :
    device_(device),
    max_frames_in_flight_(2), 
    image_available_semaphores_(CreateSemaphores()),
    render_finished_semaphores_(CreateSemaphores()),
    in_flight_fences_(CreateFences())  {
    base::Log::Info("Renderer: sync manager initialized");
}

SyncManager::~SyncManager() {
    base::Log::Info("Renderer: sync manager shutting down...");
}

void SyncManager::DeviceWaitIdle() const {
    vkDeviceWaitIdle(device_);
}

void SyncManager::UpdateFrameInFlight() {
    // In case max_frames_in_flight_ == 2 , current frame is either 0 or 1
    current_frame_in_flight_ = (current_frame_in_flight_ + 1) % max_frames_in_flight_;
}

const VkSemaphore& SyncManager::GetImageAvailableSemaphore() const {
    return image_available_semaphores_.at(current_frame_in_flight_).get()->Get();
}

const VkSemaphore& SyncManager::GetRenderFinishedSemaphore() const {
    return render_finished_semaphores_.at(current_frame_in_flight_).get()->Get();
}

const VkFence& SyncManager::GetInFlightFence() const {
    return in_flight_fences_.at(current_frame_in_flight_).get()->Get();
}

void SyncManager::WaitForInFlightFence() const {
    in_flight_fences_.at(current_frame_in_flight_).get()->WaitFor();
    in_flight_fences_.at(current_frame_in_flight_).get()->Reset();
}

// Create array of empty semaphores on heap, so only pointers are moved
std::vector<std::unique_ptr<vlk::Semaphore>> SyncManager::CreateSemaphores() const {
    std::vector<std::unique_ptr<vlk::Semaphore>> semaphores;
    for (uint32_t i = 0; i < max_frames_in_flight_; i++) {
        semaphores.push_back(std::make_unique<vlk::Semaphore>(device_));
    }
    return semaphores;
}

std::vector<std::unique_ptr<vlk::Fence>> SyncManager::CreateFences() const {
    std::vector<std::unique_ptr<vlk::Fence>> fences;
    for (uint32_t i = 0; i < max_frames_in_flight_; i++) {
        fences.push_back(std::make_unique<vlk::Fence>(device_)); 
    }
    return fences;
}

}; // renderer
