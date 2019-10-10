#include "scene_manager.h"
#include <base/log.h>

namespace scene {
SceneManager::SceneManager(renderer::Renderer& renderer, const Scene& scene) :
    renderer_(renderer),
    scene_(scene),
    view_(renderer) {
    // Fully initialize shader resources gathered from all renderable objects
    // Should be called before any descriptor set is created
    renderer_.GetShaderResources().Finalize();
    // Per view descriptor set (descriptor pool must be created)
    view_.InitDescriptorSet();
    // After descriptor pool has been created init descriptor sets
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->InitDescriptorSets();
    };
    base::Log::Info("Scene: scene manager initialized");
    renderer_.GetMemoryAllocator().DebugPrint();
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
    // Update uniform buffers
    view_.UpdateUniformBuffer(current_frame_id);
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->UpdateUniformBuffer(current_frame_id);
    };
    // Command buffer recording
    renderer_.BeginRecordCurrentCommandBuffer();
    // Bind per-view resources that later will be used by object for rendering
    view_.BindDescriptorSet(renderer_.GetCurrentCommandBuffer(), current_frame_id);
    // Add to command buffer object-by-object
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer(), current_frame_id);
    }
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}

}; // scene