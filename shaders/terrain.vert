#version 450

// Per-ciew uniform buffer
layout(set = 0, binding = 0) uniform UniformBufferView {
    mat4 view_from_world;
	mat4 projection_from_view;
} u_v;

// Per-object set
// Uniform buffer
layout(set = 1, binding = 0) uniform UniformBufferObject {
	mat4 world_from_local;
} u;
// Texture sampler
// Texture data is unsigned integer, but in case of texelFetch() sampler is not used 
layout(set = 1, binding = 1) uniform usampler2D u_height_map;

// in
layout(location = 0) in ivec2 a_local_position;
// out
layout(location = 0) out vec3 o_color;
// TODO make out as structure
//layout(location = 1) out vec2 o_tex_coord;

void main() {
	uint height = texelFetch(u_height_map, a_local_position, 0).r;
	//float height = texture(u_height_map, vec2(a_local_position.x / 1025.0, a_local_position.y / 1025.0)).r;
	
	vec4 local_position = vec4(a_local_position.x, height*0.0001, a_local_position.y, 1.0);
	gl_Position = u_v.projection_from_view * u_v.view_from_world * u.world_from_local * local_position;
    o_color = vec3(1.0, 1.0, 1.0);
	//o_tex_coord = a_tex_coord;
}