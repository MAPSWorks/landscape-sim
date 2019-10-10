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
};