#include "scene_manager.h"
#include <base/log.h>

#include <glm/gtc/matrix_transform.hpp>

namespace scene {
SceneManager::SceneManager(renderer::Renderer& renderer, const Scene& scene) :
    renderer_(renderer),
    descriptor_set_layout_view(InitDescriptorSetLayout()),
    uniform_buffer_id(renderer_.GetShaderResources().AddUniformBuffer("uniform buffer view", sizeof(UniformBufferView))),
    dummy_view_pipeline_layout(CreatePipelineLayout()),
    scene_(scene)
{
    base::Log::Info("Scene: scene manager initialized");
    renderer_.GetMemoryAllocator().DebugPrint();
    // Fully initialize shader resources gathered from all renderable objects
    renderer_.GetShaderResources().Finalize();

    // Add descriptor sets (inside sets for all frame-in-flights are created)
    descriptor_set_view_id = renderer_.GetShaderResources().AddDescriptorSet(descriptor_set_layout_view.Get());
    // Bind uniform buffer to descriptor set
    renderer_.GetShaderResources().UpdateDescriptorSetWithUniformBuffer(descriptor_set_view_id, uniform_buffer_id);

    
    // After descriptor pool has been created init descriptor sets
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->InitDescriptorSets();
    };
}

// Update the state of the world
// NOTE: updating frequency is not the same as frame rendering frequency
void SceneManager::Update() const {
    scene_.GetContents().camera->Update();
}

// Render the current state of the world
void SceneManager::RenderFrame() const {
    // Target hot loop
    // 0. Bind view/scene resources - camera, environment, (set=0)
    // Foreach shader:
    //  Bind shader pipeline
    //  1. Bind shader resources - shader control values
    //  Foreach material:
    //   2. Bind material resources - material parameters and textures
    //   Foreach object:
    //      3. Bind object resources - object transforms
    //      Draw object

    auto current_frame_id = renderer_.FrameBegin();

    UpdateUniformBuffer(current_frame_id);

    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->UpdateUniformBuffer(current_frame_id);
    };
    renderer_.BeginRecordCurrentCommandBuffer();

    // bind view set (set=0) so it is not to be rebound for each object that has compatable layout
    // dummy layout is used to simulate what set=0 wil be for other objects

    renderer_.GetCurrentCommandBuffer().BindGraphicsDescriptorSet(renderer_.GetShaderResources().GetDescriptorSet(descriptor_set_view_id, current_frame_id).Get(),
        dummy_view_pipeline_layout);
   
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer(), current_frame_id);
    }
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}

const renderer::vlk::DescriptorSetLayout& SceneManager::InitDescriptorSetLayout() const {
    std::vector<renderer::vlk::DescriptorSetLayout::Binding> bindings;
    renderer::vlk::DescriptorSetLayout::Binding binding;
    // Binding index (location), corresponds to layout(binding = n)  in shader
    binding.index = 0;
    binding.type = renderer::vlk::DescriptorType::kUniformBuffer;
    binding.stage = renderer::vlk::ShaderStage::kVertex;
    binding.count = 1;
    bindings.push_back(binding);
    return renderer_.GetShaderResources().GetDescriptorSetLayoutCache().AddDescriptorSetLayout(bindings);
}

void SceneManager::UpdateUniformBuffer(t::U32 frame_id) const {
    UniformBufferView ubo{};
    ubo.view_from_world = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.projection_from_view = glm::perspective(glm::radians(45.0f), 800 / 600.f, 0.1f, 10.0f);
    ubo.projection_from_view[1][1] *= -1;
    renderer_.GetShaderResources().GetkUniformBuffer(uniform_buffer_id, frame_id).Update(&ubo);
}

VkPipelineLayout SceneManager::CreatePipelineLayout() const {
    VkPipelineLayoutCreateInfo pipeline_layout_create_info{};
    pipeline_layout_create_info.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    // Descriptor set layout
    std::vector<VkDescriptorSetLayout> layouts = { descriptor_set_layout_view.Get() };
    pipeline_layout_create_info.setLayoutCount = 1;
    pipeline_layout_create_info.pSetLayouts = layouts.data();
    // Push constants
    pipeline_layout_create_info.pushConstantRangeCount = 0; // Optional 
    pipeline_layout_create_info.pPushConstantRanges = nullptr; // Optional
    VkPipelineLayout pipeline_layout;
    renderer::vlk::ErrorCheck(vkCreatePipelineLayout(renderer_.GetContext().device.Get(), &pipeline_layout_create_info,
        nullptr, &pipeline_layout));
    return pipeline_layout;
}
}; // scene