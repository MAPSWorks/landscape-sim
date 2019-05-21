#pragma once
#include <vector>
#include <memory>
#include <base/types.h>
#include <objects/types.h>
#include <objects/i_object.h>

namespace scene {
// Types
using UniqueObjectVector = std::vector<std::unique_ptr<object::IObject>>;

// Structures
// Settings for the scene
struct Settings {
    // ...
};
// Structure to describe objects in scene without knowing the details of them
struct ObjectDescription {
    // Object type
    object::Type type;
    t::Vec3 world_position{ 0.0 };
    // orientation, scale ...
};
// Describes scene and it's contents
struct Description {
    // World point from which scene is initially viewed
    t::Vec3 camera_position{ 0.0 };
    // Objects in scene
    std::vector<ObjectDescription> objects;
    // Lights ...
    // Weather ...
};
};