#pragma once
#include <base/types.h>

namespace scene {
// How many meters are meant by single graphics unit.
static constexpr t::F32 kMetersPerUnit = 1000.0f;
// Layout for shader resources
// Set number of a given descriptor set in shader (set=n)
enum DescruptorSetSlotId {
    kPerView = 0,
    kPerObject = 1
};
// Bindings per descriptor set.
// To set index of a binding in a descriptor set (binding=n)
enum DescriptorBidingId {
    kUniformBuffer = 0,
    kCombinedImageSampler = 1
};
// Constants
// World up vector
constexpr t::Vec3 kWorldUpVec = t::Vec3(0.0, 1.0, 0.0);
};