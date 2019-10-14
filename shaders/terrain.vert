#version 450

// Per-ciew uniform buffer
layout(set=0, binding = 0) uniform UniformBufferView {
    mat4 view_from_world;
	mat4 projection_from_view;
} u_v;

// Per-object uniform buffer
layout(set=1, binding = 0) uniform UniformBufferObject {
	mat4 world_from_local;
} u;

// in
layout(location = 0) in vec3 a_local_position;
layout(location = 1) in vec3 a_color;
// out
layout(location = 0) out vec3 o_color;

void main() {
	gl_Position = u_v.projection_from_view * u_v.view_from_world * u.world_from_local * vec4(a_local_position, 1.0);
    o_color = a_color;
}