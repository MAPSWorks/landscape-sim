#version 450
#extension GL_ARB_separate_shader_objects : enable

layout(location = 0) in vec2 a_local_position;
layout(location = 1) in vec3 a_color;

layout(location = 0) out vec3 o_color;

void main() {
    gl_Position = vec4(a_local_position, 0.0, 1.0);
    o_color = a_color;
}