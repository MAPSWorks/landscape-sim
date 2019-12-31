#pragma once
#include <glm/glm.hpp>

// Common types
namespace t {
using F32 = float;
using F64 = double;
using U16 = uint16_t;
using U32 = uint32_t;
using U64 = uint64_t;
using I16 = int16_t;
using I32 = int32_t;
using UC = unsigned char;
using US = unsigned short;
// Store data that require width and height
struct Size32 {
    Size32(U32 w = 0, U32 h = 0) : width(w), height(h) {};
    U32 width;
    U32 height;
};
struct Size16 {
    Size16(U16 w = 0, U16 h = 0) : width(w), height(h) {};
    U16 width;
    U16 height;
};
// GLM linear math types aliasing
using Vec2 = glm::tvec2<F32>;
using Vec3 = glm::tvec3<F32>;
using Vec4 = glm::tvec4<F32>;
using Vec2u16 = glm::tvec2<U16>;
using Vec3u32 = glm::tvec3<U32>;
using Vec2i16 = glm::tvec2<I16>;
using Vec2i32 = glm::tvec2<I32>;
using Vec3i32 = glm::tvec3<I32>;
using Mat3 = glm::tmat3x3<F32>;
using Mat4 = glm::tmat4x4<F32>;
// GLM type specialization
constexpr Mat4 kMat4Identoty = Mat4(1.0f);
// Constants
constexpr t::F64 kPi = 3.14159265359;
constexpr t::F64 k2Pi = 2 * kPi;
constexpr t::F64 kPi2 = kPi / 2;
// Type compile-time checks
static_assert(sizeof(F32) == 4);
static_assert(sizeof(F64) == 8);
static_assert(sizeof(UC) == 1);
static_assert(sizeof(US) == 2);
}; // t