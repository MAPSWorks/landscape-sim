#pragma once

namespace object {
// Enumeration of object types available for application
enum class Type {
    // Simple triangle, used for testing
    kTriangle = 0,
    // Triangle , data comes from vertex buffer
    kTriangleVB = 1,
    // Rectangle , data comes from vertex buffer and index buffer
    kRectangle = 2,
};
}