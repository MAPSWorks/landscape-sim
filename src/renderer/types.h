#pragma once
#include <vector>
#include <vulkan/vulkan.h>
#include <base/types.h>

// Renderer common types
namespace renderer {
// Types

// Structures
// Vertex attribute structures
// Attribute location in vertex shader (should match layout(location = n) in)
constexpr uint32_t VertexShaderPositionLocation = 0;
constexpr uint32_t VertexShaderColorLocation = 1;
struct VertexPos2dColor {
    t::Vec2 position;
    t::Vec3 color;
    // Vertex binding defines a collection of data taken from a vertex buffer bound to a 
    // selected index
    // This binding is used as a numbered source of data for vertex attributes.
    static std::vector<VkVertexInputBindingDescription> GetBindingDescription() {
        std::vector<VkVertexInputBindingDescription> binding_descr(1);
        // From which binding data is taken from
        binding_descr[0].binding = 0;
        // Jump from vertex to vertex
        binding_descr[0].stride = sizeof(VertexPos2dColor);
        // Fetched per vertex
        binding_descr[0].inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
        return binding_descr;
    }
    // Describes how to get vertex attributes from a chunk of data described by vertex input binding
    static std::vector<VkVertexInputAttributeDescription> GetAttributeDescriptions() {
        std::vector<VkVertexInputAttributeDescription> attribute_descr(2);
        // From which binding data comes from
        const uint32_t binding = 0;
        attribute_descr[0].binding = binding;
        // location= in the Vertex shader
        attribute_descr[0].location = VertexShaderPositionLocation;
        attribute_descr[0].format = VK_FORMAT_R32G32_SFLOAT;
        attribute_descr[0].offset = offsetof(VertexPos2dColor, position);
        attribute_descr[1].binding = binding;
        attribute_descr[1].location = VertexShaderColorLocation;
        attribute_descr[1].format = VK_FORMAT_R32G32B32_SFLOAT;
        attribute_descr[1].offset = offsetof(VertexPos2dColor, color);
        return attribute_descr;
    }
};
};