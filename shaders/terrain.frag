#version 450

// In
layout(location = 0) in vec3 i_color;
layout(location = 1) in vec3 i_normal;
// Out
layout(location = 0) out vec4 out_color;

void main() {
	// Calculate diffuse component
	const vec3 light_color = vec3(1.0, 1.0, 1.0); 
	const vec3 reverse_light_direction = normalize(vec3(1.0, -1.0, 0.0)) * -1.0; 
	float intensity = max(dot(i_normal, reverse_light_direction), 0.0);
	vec3 diffuse = intensity * light_color;
	// Total lighting
	vec3 lit_color = i_color * diffuse;
	out_color = vec4(lit_color, 1.0);
}