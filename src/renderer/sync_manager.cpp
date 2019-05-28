#include "sync_manager.h"
#include <vulkan/vulkan.h>
#include <base/log.h>

namespace renderer {
SyncManager::SyncManager(const VkDevice& device, uint32_t max_frames_in_flight) :
    device_(device),
    max_frames_in_flight_(max_frames_in_flight),
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
    // Current frame will be always set to either 0 or 1 for 2 images in swapchain
    current_frame_in_flight_ = (current_frame_in_flight_ + 1) % max_frames_in_flight_;
}

const VkSemaphore& SyncManager::GetImageAvailableSemaphore() const {
    return image_available_semaphores_.at(current_frame_in_flight_).Get();
}

const VkSemaphore& SyncManager::GetRenderFinishedSemaphore() const {
    return render_finished_semaphores_.at(current_frame_in_flight_).Get();
}

const VkFence& SyncManager::GetInFlightFence() const {
    return in_flight_fences_.at(current_frame_in_flight_).Get();
}

void SyncManager::WaitForInFlightFence() const {
    in_flight_fences_.at(current_frame_in_flight_).WaitFor();
    in_flight_fences_.at(current_frame_in_flight_).Reset();
}

// Deque is used because objects have no copy or move constructors
// Alternatives - store unique_ptr in vector or define move constructor
std::deque<vlk::Semaphore>  SyncManager::CreateSemaphores() const {
    std::deque<vlk::Semaphore> semaphores;
    for (uint32_t i = 0; i < max_frames_in_flight_; i++) {
        semaphores.emplace_back(device_);
    }
    return semaphores;
}

std::deque<vlk::Fence> SyncManager::CreateFences() const {
    std::deque<vlk::Fence> fences;
    for (uint32_t i = 0; i < max_frames_in_flight_; i++) {
        fences.emplace_back(device_);
    }
    return fences;
}

}; // renderer
