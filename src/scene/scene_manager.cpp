#include "scene_manager.h"
#include <memory>
#include <base/log.h>
#include <renderable/types.h>
#include <renderable/triangle.h>
#include <renderable/triangle_vb.h>
#include <renderable/rectangle.h>

namespace scene {
SceneManager::SceneManager(renderer::Renderer& renderer, const Scene& scene) :
    renderer_(renderer),
    scene_(scene) {
    base::Log::Info("Scene: scene manager initialized");
    renderer_.GetMemoryAllocator().DebugPrint();
}

SceneManager::~SceneManager() {
    base::Log::Info("Scene: scene manager shutting down..."); 
}

// Update the state of the world
// NOTE: updating frequency is not the same as frame rendering frequency
void SceneManager::Update() const {

}

// Render the current state of the world
void SceneManager::RenderFrame() const {
    renderer_.FrameBegin();
    renderer_.BeginRecordCurrentCommandBuffer();
    /*
    for (const auto& renderable : scene_.objects) {
        renderable->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer());
    }*/
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}

// Make objects in heap and store their pointers in base class pointers in vector
// TODO: could use static factory method in IRenderable class to create objects depending on type
// but we dont know how many and what kind of parameters will bee needed for each subclass renderable
UniqueObjectVector SceneManager::InitObjects(const std::vector<ObjectDescription>& object_descriptions) {
    UniqueObjectVector objects;
    for (const auto& description : object_descriptions) {
        switch (description.type) {
        case renderable::Type::kTriangle :
            objects.push_back(std::make_unique<renderable::Triangle>(renderer_));
            base::Log::Info("Scene: Triangle object added to scene");
            break;
        case renderable::Type::kTriangleVB:
            objects.push_back(std::make_unique<renderable::TriangleVB>(renderer_));
            base::Log::Info("Scene: Triangle vb object added to scene");
            break;
        case renderable::Type::kRectangle:
            objects.push_back(std::make_unique<renderable::Rectangle>(renderer_));
            base::Log::Info("Scene: Rectangle object added to scene");
            break;
        default:
            base::Log::Error("Scene: ERROR: object of unknown type in scene decription");
            break;
        }
    }
    return objects;
}
}; // scene