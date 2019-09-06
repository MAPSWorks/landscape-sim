#pragma once
#include <vector>
#include <string>
#include <memory>
#include <base/types.h>
#include <renderable/types.h>
#include <renderable/i_renderable.h>

namespace scene {
// Types
using UniqueObjectVector = std::vector<std::unique_ptr<renderable::IRenderable>>;

// Structures

// Structure to describe objects in scene without knowing the details of them
struct ObjectDescription {
    // Object type
    renderable::Type type;
    t::Vec3 world_position{ 0.0 };
    // orientation, scale ...
};
/*
// Describes scene and it's contents
struct Description {
    // World point from which scene is initially viewed
    t::Vec3 camera_position{ 0.0 };
    // Objects in scene
    std::vector<ObjectDescription> objects;
    // Lights ...
    // Weather ...
};
*/
};