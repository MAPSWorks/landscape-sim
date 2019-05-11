#pragma once
#include <glm/glm.hpp>

// Common types
namespace t {
using Real32 = float;
using Real64 = double;
struct Size {
    Size(uint32_t w = 0, uint32_t h = 0) : width(w), height(h) {};
    uint32_t width;
    uint32_t height;
};
// Redefine vector and matrix types for clarity
using Vec3 = glm::tvec3<Real32>;
using Vec2 = glm::tvec2<Real32>;
using Mat4 = glm::tmat4x4<Real32>;
}; // t