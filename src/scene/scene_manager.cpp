#include "scene_manager.h"
#include <base/log.h>

namespace scene {
SceneManager::SceneManager(renderer::Renderer& renderer, const Scene& scene) :
    renderer_(renderer),
    scene_(scene) {
    base::Log::Info("Scene: scene manager initialized");
    renderer_.GetMemoryAllocator().DebugPrint();
    // Fully initialize shader resources gathered from all renderable objects
    renderer_.GetShaderResources().Finalize();
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

    t::U32 current_frame_id = renderer_.FrameBegin();
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->UpdateUniformBuffer(current_frame_id);
    };
    renderer_.BeginRecordCurrentCommandBuffer();
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer(), current_frame_id);
    }
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}
}; // scene