#pragma once
#include <vector>
#include <memory>
#include <base/types.h>
#include "vlk/types.h"
#include "vlk/device.h"
#include "descriptor_set_layout_cache.h"
#include "vlk/memory_allocator.h"
#include "vlk/uniform_buffer.h"
#include "vlk/descriptor_pool.h"
#include "vlk/descriptor_set.h"
#include "frame_manager.h"

// Store buffers and resource descriptors to be used as shader resources
// Has copies per in-flight frame.
// Descriptor mechanism:
//  Descriptor - describes a resource to shader (UniformBuffer, texture etc)
//  Descriptor set - a collection of descriptors, this is what is bound to pipeline if we want to
//                  pass resources to shader.
//  Descriptor set layout - is like a type for descriptor set, it describes the set by defining multiple
//                         descriptors within it by bindings. 
//                         (set=0, binding=0) Uniform
//                         (set=0, binding=1) Sampler etc..
//  Pipeline layout - upon pipeline creation we need to tell piepline what descriptor set layouts
//                   are going to be used in piepline (and push constants).
namespace renderer {
class ShaderResources {
public:
    // Index of it's place into collection
    using UniformBufferId = t::U64;
    using DescrSetId = t::U64;
    // frames_in_flight - how many frames-in-flight is used and how many duplicates
    //                    of shader resources to create
    ShaderResources(const VkDevice& device, const vlk::MemoryAllocator& allocator, t::U32 frames_in_flight);
    DescriptorSetLayoutCache& GetDescriptorSetLayoutCache();
    // Add uniform buffer to per-frame data.
    // Copies of buffer for each frame-i-flight are made
    // Returns and index into container of a position of inserted buffer.
    // Note taht we cant return reference from buffer because there are 
    // multiple of them.. This index should be guaranteed not to change over
    // course of the programm.
    UniformBufferId AddUniformBuffer(std::string_view name, vlk::BufferSize buffer_size);
    // Get a uniform buffer.
    // Index - represents index into all paralel containers and is initially returned from AddUniformBuffer()
    // frame_in_flight_id - which buffer to get for current frame.
    const vlk::UniformBuffer& GetkUniformBuffer(UniformBufferId index, FrameManager::FrameId frame_in_flight_id) const;
    // Add descriptor set to per-frame data.
    // Descriptor sets are allocated from respective per-frame pool.
    // Copies of set for each frame-i-flight are made
    // Returns and index into container of a position of inserted set.
    // Note taht we cant return reference to set because there are 
    // multiple of them.. This index should be guaranteed not to change over
    // course of the programm.
    DescrSetId AddDescriptorSet(const VkDescriptorSetLayout& layout);
    // Get a descriptor set
    // Index - represents index into all paralel containers and is initially returned from AddDescriptorSet()
    // frame_in_flight_id - which buffer to get for current frame.
    const vlk::DescriptorSet& GetDescriptorSet(DescrSetId index, FrameManager::FrameId frame_in_flight_id) const;
    // Set descriptor pool from cached descriptor set layouts.
    // NOTE: Ths function should be called after all object requiring shader resources are initialized!
    // and before any descriptor set is created.
    void Finalize();
    // Bind uniform buffer to descriptor set
    // If buffer_zize is passed 0, the whole buffer size is bound to descriptor set
    void UpdateDescriptorSetWithUniformBuffer(DescrSetId descriptor_set_id, UniformBufferId uniform_buffer_id, t::U64 buffer_size=0) const;
private:
    // Data that is used per frame and therefore needs a copy for each 
    // frame-in-flight.
    struct PerFrameShaderResourceData {
        // Stores uniform data for shaders 
        std::vector<std::unique_ptr<const vlk::UniformBuffer>> uniform_buffers;
        // Each frame-in-flight gets it's own descriptor pool.
        // Descriptor pool is an object from which we allocate descriptor sets
        // therefore it can be initialized only after we know how many descriptor sets are needed.
        // Since it is lazy initialized store it in pointer.
        std::unique_ptr<const vlk::DescriptorPool> descriptor_pool;
        // Descriptor set is a collection of descriptors that are bound together
        // and accessed in shader as (set=n)
        std::vector<std::unique_ptr<const vlk::DescriptorSet>> descriptor_sets;
    };
    // Set descriptor pool
    void SetDescriptorPool();
    // Store reference to device to be used for object creation later
    const VkDevice& device_;
    // Reference to memeory allocator to allocate buffers
    const vlk::MemoryAllocator& allocator_;
    // To not recreate already existing set layouts store them and retrieve from cache
    DescriptorSetLayoutCache descriptor_set_layout_cache_;
    // Per frame-in-flight data
    // Container size if the same as frame-in-flight number
    std::vector<PerFrameShaderResourceData> per_frame_shader_resources_;
};
}; // renderer