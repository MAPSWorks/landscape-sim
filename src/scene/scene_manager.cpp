#include "scene_manager.h"
#include <memory>
#include <base/log.h>
#include <objects/types.h>
#include <objects/triangle.h>
#include <objects/triangle_vb.h>

namespace scene {
SceneManager::SceneManager(const Settings& settings, const Description& scene_description,
    renderer::Renderer& renderer) :
    renderer_(renderer),
    camera_world_position_(scene_description.camera_position),
    objects_(InitObjects(scene_description.objects))  {
    base::Log::Info("Scene: scene manager initialized");
    renderer_.GetMemoryAllocator().DebugPrint();
}

SceneManager::~SceneManager() {
    base::Log::Info("Scene: scene manager shutting down..."); 
}

void SceneManager::RenderFrame() const {
    renderer_.FrameBegin();
    renderer_.BeginRecordCurrentCommandBuffer();
    for (const auto& object : objects_) {
        object->AppendCommandBuffer(renderer_.GetCurrentCommandBuffer());
    }
    renderer_.EndRecordCurrentCommandBuffer();
    renderer_.FrameEnd();
}

// Make objects in heap and store their pointers in base class pointers in vector
// TODO: could use static factory method in IObject class to create objects depending on type
// but we dont know how many and what kind of parameters will bee needed for each subclass object
UniqueObjectVector SceneManager::InitObjects(const std::vector<ObjectDescription>& object_descriptions) {
    UniqueObjectVector objects;
    for (const auto& description : object_descriptions) {
        switch (description.type) {
        case object::Type::kTriangle :
            objects.push_back(std::make_unique<object::Triangle>(renderer_));
            base::Log::Info("Scene: Triangle object added to scene");
            break;
        case object::Type::kTriangleVB:
            objects.push_back(std::make_unique<object::TriangleVB>(renderer_));
            base::Log::Info("Scene: Triangle vb object added to scene");
            break;
        default:
            base::Log::Error("Scene: ERROR: object of unknown type in scene decription");
            break;
        }
    }
    return objects;
}
}; // scene