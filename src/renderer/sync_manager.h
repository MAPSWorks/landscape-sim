#pragma once
#include <vector>
#include <memory>
#include "vlk/semaphore.h"
#include "vlk/fence.h"

// Handles and stores rendering synchronization objects
namespace renderer {
class SyncManager {
public:
    SyncManager(const VkDevice& device);
    ~SyncManager();
    void DeviceWaitIdle() const;
    // Call this every frame
    void UpdateFrameInFlight();
    // Semaphore is picked according to current frame in flight
    const VkSemaphore& GetImageAvailableSemaphore() const;
    const VkSemaphore& GetRenderFinishedSemaphore() const;
    const VkFence& GetInFlightFence() const;
    // Wait for current frame-in-flight to be signaled and reset it after
    void WaitForInFlightFence() const;
private:
    std::vector<std::unique_ptr<vlk::Semaphore>> CreateSemaphores() const;
    std::vector<std::unique_ptr<vlk::Fence>> CreateFences() const;
    const VkDevice& device_;;
    // Rendering and presenting synchronization
    // If we wait for queue to finish presenting after every frame, it means
    // first shader stages on GPU are idle, we could use them already while
    // we wait for previous frame to completely finish to present.
    // This constant tells maximum number if frames that can be processed simultaniously
    const uint32_t max_frames_in_flight_;
    // To keep track of rendeing frames
    uint32_t current_frame_in_flight_ = 0;
    // Each frame should have their own set of semaphores (gpu to gpu)
    const std::vector<std::unique_ptr<vlk::Semaphore>> image_available_semaphores_;
    const std::vector<std::unique_ptr<vlk::Semaphore>> render_finished_semaphores_;
    // To bound the render processing to max_frames_in_flight_ we need to synchronize cpu with gpu
    // before we did it simply with CkWaitQueueIdle, now we use fences to hold execution of only one
    // frame, while other one already starts running
    const std::vector<std::unique_ptr<vlk::Fence>> in_flight_fences_;
};
}; //renderer