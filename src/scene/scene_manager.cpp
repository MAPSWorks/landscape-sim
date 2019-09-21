#include "scene_manager.h"
#include <base/log.h>

namespace scene {
SceneManager::SceneManager(renderer::Renderer& renderer, const Scene& scene) :
    renderer_(renderer),
    scene_(scene) {
    base::Log::Info("Scene: scene manager initialized");
    renderer_.GetMemoryAllocator().DebugPrint();
}

// Update the state of the world
// NOTE: updating frequency is not the same as frame rendering frequency
void SceneManager::Update() const {

}

// Render the current state of the world
void SceneManager::RenderFrame() const {
    renderer_.FrameBegin();
    renderer_.BeginRecordCurrentCommandBuffer();
    for (const auto& renderable : scene_.GetContents().renderables) {
        renderable->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer());
    }
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}
}; // scene