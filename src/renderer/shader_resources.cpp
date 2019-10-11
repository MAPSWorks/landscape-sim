#include "shader_resources.h"
#include <base/log.h>

namespace renderer {
ShaderResources::ShaderResources(const VkDevice& device, const vlk::MemoryAllocator& allocator, 
    t::U32 frames_in_flight) :
    device_(device),
    allocator_(allocator),
    descriptor_set_layout_cache_(device),
    per_frame_shader_resources_(frames_in_flight) {
    base::Log::Info("Renderer: shader resource initialized, frame duplicate count - ", 
        per_frame_shader_resources_.size());
}

DescriptorSetLayoutCache& ShaderResources::GetDescriptorSetLayoutCache() {
    return descriptor_set_layout_cache_;
}

// Buffer is created for all paralel frame-in-flight resources
ShaderResources::UniformBufferId ShaderResources::AddUniformBuffer(std::string_view name, vlk::BufferSize buffer_size) {
    for (auto& resource : per_frame_shader_resources_) {
        resource.uniform_buffers.push_back(std::make_unique<const vlk::UniformBuffer>(name, allocator_, buffer_size));
    };
    // Return index just inserted
    return per_frame_shader_resources_.at(0).uniform_buffers.size() - 1;
}

const vlk::UniformBuffer& ShaderResources::GetkUniformBuffer(ShaderResources::UniformBufferId index, FrameId frame_in_flight_id) const {
   return *per_frame_shader_resources_.at(frame_in_flight_id).uniform_buffers.at(index);
}

// Descriptor set is created for all paralel frame-in-flight resources
ShaderResources::DescrSetId ShaderResources::AddDescriptorSet(const VkDescriptorSetLayout& layout) {
    for (auto& resource : per_frame_shader_resources_) {
        resource.descriptor_sets.push_back(std::make_unique<const vlk::DescriptorSet>(device_, resource.descriptor_pool->Get(), layout));
    };
    // Return index just inserted
    return per_frame_shader_resources_.at(0).descriptor_sets.size() - 1;
}

const vlk::DescriptorSet& ShaderResources::GetDescriptorSet(ShaderResources::DescrSetId index, FrameId frame_in_flight_id) const {
    return *per_frame_shader_resources_.at(frame_in_flight_id).descriptor_sets.at(index);
}

void ShaderResources::Finalize() {
    SetDescriptorPool();
}

// Update descriptor set for all frame-in-flight copies
void ShaderResources::UpdateDescriptorSetWithUniformBuffer(ShaderResources::DescrSetId descriptor_set_id, ShaderResources::UniformBufferId uniform_buffer_id, t::U64 buffer_size) const {
    // Bind actual uniform buffer to descriptor set
    for (t::U32 i = 0; i < per_frame_shader_resources_.size(); ++i) {
        GetDescriptorSet(descriptor_set_id, i).UpdateUniformBuffer(GetkUniformBuffer(uniform_buffer_id, i).Get(), buffer_size);
    }
}

// Initialize descriptor pool for each frame in flight
void ShaderResources::SetDescriptorPool() {
    // Retrieve needed information from cache statistic after all descriptor layout have been created
    const auto& descriptor_set_stats = descriptor_set_layout_cache_.GetDescrikptorSetStats();
    // Describe how many descriptors of each type will be available from the pool.
    std::vector<vlk::DescriptorPool::PoolSize> pool_sizes;
    for (const auto& type_stats : descriptor_set_stats.descriptor_type_count) {
        vlk::DescriptorPool::PoolSize pool_size;
        // Stats are stored in hash-map where key is type and value is it's total count
        pool_size.type = type_stats.first;
        pool_size.descriptor_count = type_stats.second;
        pool_sizes.push_back(pool_size);
    }

    for (auto& resource : per_frame_shader_resources_) {
        resource.descriptor_pool = std::make_unique<const vlk::DescriptorPool>(device_, pool_sizes, descriptor_set_stats.set_count);
    }
}
}; // renderer