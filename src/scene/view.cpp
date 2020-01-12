#include "view.h"
#include <vector>
#include <base/log.h>
#include "types.h"

namespace scene {
View::View(renderer::Renderer& renderer) :
    renderer_(renderer),
    descr_set_layout_(AddDescrSetLayout()),
    uniform_buffer_view_id_(AddUniformBufferViewId()),
    uniform_buffer_environment_id_(AddUniformBufferViewEnvironment()),
    pipeline_layout_dummy_(renderer_.GetContext().device.Get(), renderer::vlk::PipelineLayout::Params{ {descr_set_layout_.Get()} }) {
    base::Log::Info("Scene: view initialized");
}

void View::InitDescriptorSet() {
    // Add descriptor sets (inside sets for all frame-in-flights are created)
    descr_set_id_ = renderer_.GetShaderResources().AddDescriptorSet(descr_set_layout_.Get());
    // Bind resources to descriptor set
    std::vector<renderer::ShaderResources::DescrSetUpdateInfo> resources_to_bind;
    {
        renderer::ShaderResources::DescrSetUpdateInfo resource;
        resource.type = renderer::vlk::DescriptorType::kUniformBuffer;
        resource.count = 1;
        resource.buffer_id = uniform_buffer_view_id_;
        resource.buffer_offset = 0;
        // Whole buffer
        resource.buffer_range = 0;
        resources_to_bind.push_back(resource);
    }
    {
        renderer::ShaderResources::DescrSetUpdateInfo resource;
        resource.type = renderer::vlk::DescriptorType::kUniformBuffer;
        resource.count = 1;
        resource.buffer_id = uniform_buffer_environment_id_;
        resource.buffer_offset = 0;
        // Whole buffer
        resource.buffer_range = 0;
        resources_to_bind.push_back(resource);
    }

    renderer_.GetShaderResources().UpdateDescriptorSet(descr_set_id_, resources_to_bind);
}

void View::UpdateUniformBuffer(renderer::FrameManager::FrameId frame_id, const ICamera& camera, const Environment& environmen) const {
    // View data
    {
        UniformDataView ubo{};
        ubo.view_from_world = camera.GetViewMatrix();
        ubo.projection_from_view = camera.GetProjectionMatrix(renderer_.GetWindow().GetAspectRatio());
        renderer_.GetShaderResources().GetkUniformBuffer(uniform_buffer_view_id_, frame_id).Update(&ubo);
    }
    // Environemnt data
    {
        UniformDataEnvironment ubo{};
        ubo.sunlight_direction = environmen.GetSun().direction;
        ubo.sunlight_color = environmen.GetSun().color;
        renderer_.GetShaderResources().GetkUniformBuffer(uniform_buffer_environment_id_, frame_id).Update(&ubo);
    }
}

// Per-view shader resources are bound once per view, so this resource is not necessery
// to bind per-object basis. 
// This function should be called at the top of a rendering loop.
void View::BindDescriptorSet(const renderer::vlk::CommandBuffer& command_buffer, renderer::FrameManager::FrameId frame_id) const {
    command_buffer.BindGraphicsDescriptorSet(renderer_.GetShaderResources().GetDescriptorSet(descr_set_id_, frame_id).Get(),
        pipeline_layout_dummy_.Get(), DescruptorSetSlotId::kPerView);
}

const renderer::vlk::DescriptorSetLayout& View::GetDescriptorSetLayout() const {
    return descr_set_layout_;
}

const renderer::vlk::DescriptorSetLayout& View::AddDescrSetLayout() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        const t::U32 view_binding = 0;
        binding.index = view_binding;
        binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
        binding.stage = renderer::vlk::ShaderStage::kVertex;
        binding.count = 1;
        bindings.push_back(binding);
    }
    {
        renderer::vlk::DescriptorSetLayout::Binding binding;
        // Binding index (location), corresponds to layout(binding = n)  in shader
        const t::U32 environment_binding = 1;
        binding.index = environment_binding;
        binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
        binding.stage = renderer::vlk::ShaderStage::kFragment;
        binding.count = 1;
        bindings.push_back(binding);
    }

    return renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(bindings);
}

t::U64 View::AddUniformBufferViewEnvironment() const {
    return renderer_.GetShaderResources().AddUniformBuffer("uniform buffer environment", sizeof(UniformDataEnvironment));
}

t::U64 View::AddUniformBufferViewId() const {
    return renderer_.GetShaderResources().AddUniformBuffer("uniform buffer view", sizeof(UniformDataView));
}
}; // scene