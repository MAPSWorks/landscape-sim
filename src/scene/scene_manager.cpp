#include "scene_manager.h"
#include <imgui/imgui.h>
#include <base/log.h>

namespace scene {
SceneManager::SceneManager(renderer::Renderer& renderer, std::string_view scene_file_name) :
    renderer_(renderer),
    view_(renderer_),
    scene_(scene_file_name, renderer_, view_) {
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
void SceneManager::Update(const platform::Input& input) {
    t::F32 dt = 0.01f;
    scene_.GetContents().camera->Move(input, dt);
}

// Render the current state of the world
void SceneManager::RenderFrame(const gui::GUI& gui) {
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
    view_.UpdateUniformBuffer(current_frame_id, *scene_.GetContents().camera);
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->UpdateUniformBuffer(current_frame_id, scene_.GetContents().environment);
    };

    // Command buffer recording
    renderer_.BeginRecordCurrentCommandBuffer();
    // Bind per-view resources that later will be used by object for rendering
    view_.BindDescriptorSet(renderer_.GetCurrentCommandBuffer(), current_frame_id);
    // Add to command buffer object-by-object
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer(), current_frame_id);
    }
    // GUI
    gui.BeginFrame();
    // Specify GUI for each renderable
    if (gui.IsEnabled()) {
        UpdateApplicationGUI();
        scene_.GetContents().environment.UpdateGUI();
        for (const auto& renderable : scene_.GetContents().renderables) {
            renderable->UpdateGUI();
        }
    }
    gui.PrepareForRendering();
    gui.AppendCommandBuffer(renderer_.GetCurrentCommandBuffer().Get());
    
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}

void SceneManager::MouseMove(const platform::Input& input) {
    scene_.GetContents().camera->Rotate(input);
}

void SceneManager::UpdateApplicationGUI() const {
    ImGui::Begin("Application");
    ImGui::Text("Average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
    ImGui::End();
}

}; // scene