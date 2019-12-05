#pragma once
#include <base/types.h>

namespace scene {
// How many meters are meant by single Vulkan unit.
// This field is integer to not mess up caluculation precision and it is unlikely
// to be set under 1 or somewhere between two integers.
static constexpr t::U32 kMetersPerUnit = 1;
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