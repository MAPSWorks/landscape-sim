#pragma once
#include <base/types.h>

namespace scene {
// Layout for shader resources
// Set number of a given descriptor set in shader (set=n)
enum DescruptorSetSlotId {
    kPerView = 0
};
// Bindings per descriptor sets.
// Per view shader resources, uniform buffer is having (binding=n)
enum ViewDescriptorBidingId {
    kUniformBuffer = 0
};
// Constants
// World up vector
constexpr t::Vec3 kWorldUpVec = t::Vec3(0.0, 1.0, 0.0);
};