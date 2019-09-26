#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(binding = 0) uniform UniformBufferObject {
	mat4 world_from_local;
    mat4 view_from_world;
	mat4 projection_from_view;
} u;

layout(location = 0) in vec2 a_local_position;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 o_color;

void main() {
	gl_Position = u.projection_from_view * u.view_from_world * u.world_from_local * vec4(a_local_position, 0.0, 1.0);
    o_color = a_color;
}