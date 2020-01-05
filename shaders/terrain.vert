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
	vec3 sunlight_direction; // not used here TODO: think about it
	vec3 sunlight_color; // not used here TODO: think about it
} u;
// Texture sampler
// Texture data is unsigned integer, but in case of texelFetch() sampler is not used 
layout(set = 1, binding = 1) uniform usampler2D u_height_map;

// in
layout(location = 0) in ivec2 a_local_position;
// out
layout(location = 0) out vec3 o_color;
layout(location = 1) out vec3 o_normal;
// TODO make out as structure

// Unnormalized normal that is calculated as cross product 
// from vectors that go from current position to adjacent positions
vec3 ComputeNormal(in ivec2 map_coords, in vec3 position) {
	// Find adjacent positions (as looking from above)
	ivec2 position_to_right =  map_coords + ivec2(1.0, 0.0);
	ivec2 position_to_top =  map_coords + ivec2(0.0, 1.0);
	// Get height of adjacent positions
	uint height_to_right = texelFetch(u_height_map, position_to_right, 0).r;
	uint height_to_top = texelFetch(u_height_map, position_to_top, 0).r;
	// Get adjacent positions in 3d
	vec3 right = vec3(position_to_right.x, height_to_right, position_to_right.y);
	vec3 top = vec3(position_to_top.x, height_to_top, position_to_top.y);
	// normal vector
	return cross(top - position, right - position);
}

void main() {
	ivec2 map_coords = a_local_position;
	uint height = texelFetch(u_height_map, map_coords, 0).r;
	vec3 local_position = vec3(map_coords.x, height, map_coords.y);
	// Calculate normal that is not scaled and normalized
	o_normal = ComputeNormal(map_coords, local_position);
	// Should also be scale just like mesh itself
	// OPTI: pass expensive normal matrix through uniform or calculate normals already
	//		 in world space (in function ComputeNormal() multiply positions with world mat)
	o_normal = mat3(transpose(inverse(u.world_from_local))) * o_normal;
	o_normal = normalize(o_normal);
	o_color = vec3(1.0, 1.0, 1.0);
	gl_Position = u_v.projection_from_view * u_v.view_from_world * u.world_from_local * vec4(local_position, 1.0);
}